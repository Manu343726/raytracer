#ifndef RAYTRACER_JOBS_WORKER_HPP
#define RAYTRACER_JOBS_WORKER_HPP

#include "pool.hpp"
#include "job.hpp"
#include "jobqueue.hpp"
#include "lockedjobqueue.hpp"
#include <thread>
#include <atomic>

namespace rt
{

namespace jobs
{

class Engine;

using WorkQueue = rt::jobs::JobQueue;

/**
 * \ingroup jobs
 * \
 */
class Worker
{
public:
    enum class Mode
    {
        Background,
        Foreground
    };

    enum class State
    {
        Idle,
        Running,
        Stopping
    };

    Worker(const std::uint64_t id, Engine* engine, std::size_t poolSize, Mode mode = Mode::Background);
    ~Worker();

    std::uint64_t id() const;
    std::thread::id threadId() const;
    bool running() const;
    void run();
    void stop();
    void submit(Job* job);
    void wait(Job* job);
    Pool& pool();
    const Pool& pool() const;
    void join();

    const std::atomic<State>& state() const;
    std::size_t totalJobsRun() const;
    std::size_t totalJobsDiscarded() const;
    std::size_t cyclesWithoutJobs() const;
    std::size_t maxCyclesWithoutJobs() const;

private:
    WorkQueue _workQueue;
    Pool _pool;
    Engine* _engine;
    std::thread _workerThread;
    std::thread::id _workerThreadId;
    Mode _mode;
    std::atomic<State> _state;
    std::size_t _totalJobsRun;
    std::size_t _totalJobsDiscarded;
    std::size_t _cyclesWithoutJobs;
    std::size_t _maxCyclesWithoutJobs;
    std::uint64_t _id;

    Job* getJob();
    void getJobs();
};

}

}

#endif // RAYTRACER_JOBS_WORKER_HPP
