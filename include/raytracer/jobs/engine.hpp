#ifndef RAYTRACER_JOBS_ENGINE_HPP
#define RAYTRACER_JOBS_ENGINE_HPP

#include "worker.hpp"
#include <random>
#include <raytracer/static_vector.hpp>

namespace rt
{

namespace jobs
{

/**
 * \ingroup jobs
 * \brief Manages a set of workers where jobs can be submitted to
 */
class Engine
{
public:
    /**
     * \brief Initializes an engine with /p workerThreads workers and
     * a maximum of \p jobsPerThread jobs per worker
     *
     * \param workerThreads The number of workers this engine will manage.
     * One of these workers will be run in the caller thread, which means only
     * n-1 workers will have background threads polling for work to do.
     * See Worker::wait().
     * \param jobsPerThread Maximum number of jobs that can be allocated by
     * thread. Once this limit is reached, the worker stops returning storage
     * for more jobs, which means no more jobs can be submitted to the worker.
     */
    Engine(
        const std::size_t               workerThreads,
        const std::vector<std::size_t>& jobsPerThread,
        const std::size_t               fallbackJobsPerThread);

    Engine(const std::size_t workerThreads, const std::size_t jobsPerThread);

    Engine(const Engine&) = delete;

    /**
     * \brief Returns one of the workers, randomnly picked from all
     * the available workers in the engine
     *
     * Note allocating and submitting jobs from a thread different from
     * the worker associated to the caller thread has undefined behavior.
     * See `threadWorker()`.
     */
    Worker* randomWorker();

    /**
     * \brief Returns the worker associated to a given thread
     *
     * \param threadId Id of the thread
     *
     * \returns A pointer to the worker which is using the given thread
     * as worker thread, nullptr if no worker is using the give thread.
     */
    Worker* findThreadWorker(const std::thread::id threadId);

    /**
     * \brief Returns the worker associated to the caller thread
     */
    Worker* threadWorker();

    /**
     * \brief Returns the total number of jobs run by the engine
     */
    std::size_t totalJobsRun() const;
    std::size_t totalJobsAllocated() const;

    const rt::static_vector<Worker>& workers() const;

private:
    rt::static_vector<Worker>                  _workers;
    std::default_random_engine                 _randomEngine;
    std::uniform_int_distribution<std::size_t> _dist;
};

} // namespace jobs

} // namespace rt

#endif // RAYTRACER_JOBS_ENGINE_HPP
