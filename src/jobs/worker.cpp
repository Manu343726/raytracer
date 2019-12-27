#include <fmt/ostream.h>
#include <raytracer/debug/profile.hpp>
#include <raytracer/jobs/engine.hpp>
#include <raytracer/jobs/worker.hpp>
#include <spdlog/spdlog.h>

using namespace rt::jobs;

Worker::Worker(
    const std::uint64_t id,
    Engine*             engine,
    std::size_t         poolSize,
    Worker::Mode        mode)
    : _workQueue{poolSize + 1},
      _pool{poolSize},
      _engine{engine},
      _mode{mode},
      _state{State::Idle},
      _totalJobsRun{0},
      _totalJobsDiscarded{0},
      _cyclesWithoutJobs{0},
      _maxCyclesWithoutJobs{0},
      _id{id}
{
}

void Worker::run()
{
    if(running())
    {
        return;
    }

    auto mainLoop = [this] {
        ZoneNamed(MainLoop, true);

        _state = State::Running;

        spdlog::debug("Background worker {} started", id());

        while(running())
        {
            ZoneNamed(WorkerStep, true);

            Job* job = getJob();

            if(job != nullptr)
            {
                if(job->run())
                {
                    ++_totalJobsRun;
                    _cyclesWithoutJobs = 0;
                }
            }
            else
            {
                ++_cyclesWithoutJobs;
                _maxCyclesWithoutJobs =
                    std::max(_cyclesWithoutJobs, _maxCyclesWithoutJobs);
            }
        }
    };

    if(_mode == Mode::Background)
    {
        _workerThread   = std::thread{mainLoop};
        _workerThreadId = _workerThread.get_id();
    }
    else
    {
        _state          = State::Running;
        _workerThreadId = std::this_thread::get_id();

        spdlog::debug("Foreground worker {} started", id());
    }
}

void Worker::stop()
{
    ZoneScoped;

    State expected = State::Running;
    while(!_state.compare_exchange_weak(expected, State::Stopping))
        ;

    spdlog::debug("Stopping worker {}...", id());
    join();
    _state = State::Idle;
    spdlog::debug(
        "Worker {0} stopped ({1} jobs were allocated [{2}%], {3} jobs were run, {4} jobs discarded, {5} max idle cycles)",
        id(),
        pool().jobs(),
        pool().jobsFactor() * 100,
        totalJobsRun(),
        totalJobsDiscarded(),
        maxCyclesWithoutJobs());
}

Worker::~Worker()
{
    stop();
}

void Worker::join()
{
    if(std::this_thread::get_id() != threadId() && _workerThread.joinable())
    {
        _workerThread.join();
    }
}

bool Worker::running() const
{
    return _state == State::Running;
}

void Worker::submit(Job* job)
{
    if(job != nullptr && !_workQueue.push(job))
    {
        job->discard();
        ++_totalJobsDiscarded;
    }
}

void Worker::wait(Job* waitJob)
{
    ZoneScoped;

    while(!waitJob->finished())
    {
        ZoneNamedN(WaitStep, "Job wait step", true);

        Job* job = getJob();

        if(job != nullptr)
        {
            job->run();
            ++_totalJobsRun;
            _cyclesWithoutJobs = 0;
        }
        else
        {
            ++_cyclesWithoutJobs;
            _maxCyclesWithoutJobs =
                std::max(_cyclesWithoutJobs, _maxCyclesWithoutJobs);
        }
    }
}

Pool& Worker::pool()
{
    return _pool;
}

const Pool& Worker::pool() const
{
    return _pool;
}

Job* Worker::getJob()
{
    ZoneScoped;

    Job* job = _workQueue.pop();

    if(job != nullptr)
    {
        return job;
    }
    else
    {
        ZoneNamedN(JonStealing, "Job stealing", true);

        // Steal job from another worker

        Worker* worker = _engine->randomWorker();

        if(worker == this)
        {
            TracyMessageL("worker yields");
            std::this_thread::yield();
            TracyMessageL("worker woke up from yield");
            return nullptr;
        }
        else
        {
            if(worker != nullptr)
            {
                spdlog::trace(
                    "Worker {} stealing work from worker {}",
                    id(),
                    worker->id());
                return worker->_workQueue.steal();
            }
            else
            {
                TracyMessageL("worker yields");
                std::this_thread::yield();
                TracyMessageL("worker woke up from yield");
                return nullptr;
            }
        }
    }
}

std::uint64_t Worker::id() const
{
    return _id;
}

std::thread::id Worker::threadId() const
{
    return _workerThreadId;
}

const std::atomic<Worker::State>& Worker::state() const
{
    return _state;
}

std::size_t Worker::totalJobsRun() const
{
    return _totalJobsRun;
}

std::size_t Worker::cyclesWithoutJobs() const
{
    return _cyclesWithoutJobs;
}

std::size_t Worker::maxCyclesWithoutJobs() const
{
    return _maxCyclesWithoutJobs;
}

std::size_t Worker::totalJobsDiscarded() const
{
    return _totalJobsDiscarded;
}
