#include <raytracer/jobs/worker.hpp>
#include <raytracer/jobs/engine.hpp>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

using namespace ray::jobs;

Worker::Worker(Engine* engine, std::size_t poolSize, Worker::Mode mode) :
    _workQueue{poolSize},
    _pool{poolSize},
    _engine{engine},
    _mode{mode},
    _state{State::Idle},
    _totalJobsRun{0},
    _totalJobsDiscarded{0},
    _cyclesWithoutJobs{0},
    _maxCyclesWithoutJobs{0}
{}

void Worker::run()
{
    if(running())
    {
        return;
    }

    auto mainLoop = [this]
    {
        _state = State::Running;

        spdlog::debug("Worker {} started", threadId());

        while(running())
        {
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
                _maxCyclesWithoutJobs = std::max(_cyclesWithoutJobs, _maxCyclesWithoutJobs);
            }
        }
    };

    if(_mode == Mode::Background)
    {
        _workerThread = std::thread{mainLoop};
        _workerThreadId = _workerThread.get_id();
    }
    else
    {
        _state = State::Running;
        _workerThreadId = std::this_thread::get_id();
    }
}

void Worker::stop()
{
    State expected = State::Running;
    while(!_state.compare_exchange_weak(expected, State::Stopping));

    spdlog::debug("Stopping worker {}...", threadId());
    join();
    _state = State::Idle;
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
    while(!waitJob->finished())
    {
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
            _maxCyclesWithoutJobs = std::max(_cyclesWithoutJobs, _maxCyclesWithoutJobs);
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
    Job* job = _workQueue.pop();

    if(job != nullptr)
    {
        return job;
    }
    else
    {
        // Steal job from another worker

        Worker* worker = _engine->randomWorker();

        if(worker == this)
        {
            std::this_thread::yield();
            return nullptr;
        }
        else
        {
            if(worker != nullptr)
            {
                spdlog::trace("Worker {} stealing work from worker {}", threadId(), worker->threadId());
                return worker->_workQueue.steal();
            }
            else
            {
               std::this_thread::yield();
               return nullptr;
            }
        }
    }
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
