#include <raytracer/jobs/engine.hpp>
#include <random>

using namespace ray::jobs;

Engine::Engine(std::size_t workerThreads, std::size_t jobsPerThread) :
    _workers{workerThreads},
    _randomEngine{std::random_device()()},
    _dist{0, workerThreads}
{
    std::size_t jobsPerQueue = jobsPerThread;
    _workers.emplace_back(this, jobsPerQueue, Worker::Mode::Foreground);

    for(std::size_t i = 1; i < workerThreads; ++i)
    {
        _workers.emplace_back(this, jobsPerQueue, Worker::Mode::Background);
    }

    for(auto& worker : _workers)
    {
        worker.run();
    }
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

Engine::~Engine()
{
    for(auto& worker : _workers)
    {
        worker.stop();
    }
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

Worker* Engine::threadWorker()
{
    static thread_local Engine* engine = this;
    static thread_local Worker* worker = findThreadWorker(std::this_thread::get_id());

    if(engine != this)
    {
        engine = this;
        worker = findThreadWorker(std::this_thread::get_id());
    }

    return worker;
}

const ray::static_vector<Worker>& Engine::workers() const
{
    return _workers;
}
