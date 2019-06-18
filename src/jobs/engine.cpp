#include <fmt/ostream.h>
#include <random>
#include <raytracer/jobs/engine.hpp>
#include <spdlog/spdlog.h>

using namespace rt::jobs;

Engine::Engine(
    const std::size_t               workerThreads,
    const std::vector<std::size_t>& jobsPerThread,
    const std::size_t               fallbackJobsPerThread)
    : _workers{workerThreads},
      _randomEngine{std::random_device()()},
      _dist{0, workerThreads}
{
    spdlog::debug(
        "Initializing engine with {} workers and [{}] jobs per worker...",
        workerThreads,
        fmt::join(jobsPerThread, ", "));

    std::size_t jobsPerQueue = fallbackJobsPerThread;

    if(jobsPerThread.size() > 0)
    {
        jobsPerQueue = static_cast<std::size_t>(jobsPerThread[0]);
    }

    _workers.emplace_back(0ull, this, jobsPerQueue, Worker::Mode::Foreground);

    for(std::size_t i = 1; i < workerThreads; ++i)
    {

        if(jobsPerThread.size() > i)
        {
            jobsPerQueue = static_cast<std::size_t>(jobsPerThread[i]);
        }
        else
        {
            jobsPerQueue = fallbackJobsPerThread;
        }

        _workers.emplace_back(i, this, jobsPerQueue, Worker::Mode::Background);
    }

    spdlog::debug("Engine initialized");

    for(auto& worker : _workers)
    {
        spdlog::debug("Starting worker {}...", worker.id());
        worker.run();
    }
}

Engine::Engine(const std::size_t workerThreads, const std::size_t jobsPerThread)
    : Engine{workerThreads,
             std::vector<std::size_t>(workerThreads, jobsPerThread),
             jobsPerThread}
{
}

Worker* Engine::randomWorker()
{
    Worker* worker = &_workers[_dist(_randomEngine)];

    if(worker->running())
    {
        return worker;
    }
    else
    {
        return nullptr;
    }
}

Worker* Engine::findThreadWorker(const std::thread::id threadId)
{
    for(auto& worker : _workers)
    {
        if(threadId == worker.threadId())
        {
            return &worker;
        }
    }

    return nullptr;
}

std::size_t Engine::totalJobsRun() const
{
    std::size_t total = 0;

    for(const auto& worker : _workers)
    {
        total += worker.totalJobsRun();
    }

    return total;
}

std::size_t Engine::totalJobsAllocated() const
{
    std::size_t total = 0;

    for(const auto& worker : _workers)
    {
        total += worker.pool().jobs();
    }

    return total;
}

Worker* Engine::threadWorker()
{
    static thread_local Engine* engine = this;
    static thread_local Worker* worker =
        findThreadWorker(std::this_thread::get_id());

    if(engine != this)
    {
        engine = this;
        worker = findThreadWorker(std::this_thread::get_id());
    }

    return worker;
}

const rt::static_vector<Worker>& Engine::workers() const
{
    return _workers;
}
