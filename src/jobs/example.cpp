#include <raytracer/jobs/engine.hpp>
#include <cstdio>
#include <iostream>
#include <thread>

using namespace ray::jobs;

void work(Job& job)
{
#ifdef RAYTRACER_JOBS_EXAMPLE_SIMULATE_WORK
    auto start = std::chrono::steady_clock::now();

    while(std::chrono::steady_clock::now() - start < std::chrono::milliseconds(1));

#ifdef RAYTRACER_JOBS_EXAMPLE_LOG_WORK
    std::printf("job %p done\n", reinterpret_cast<void*>(&job));
#endif // RAYTRACER_JOBS_EXAMPLE_LOG_WORK

#endif // RAYTRACER_JOBS_EXAMPLE_SIMULATE_WORK
}

int main(int argc, char** argv)
{
    constexpr std::size_t threads = 4;
    const std::size_t childJobs = (argc > 1 ? std::atoi(argv[1]) : 65*100);
    const std::size_t secondLevelChildJobs = (argc > 2 ? std::atoi(argv[2]) : 10);
    const std::size_t totalJobs = childJobs + (childJobs * secondLevelChildJobs);


    Engine engine{threads, totalJobs};
    Worker* worker = engine.threadWorker();

    auto start = std::chrono::high_resolution_clock::now();

    Job* root = worker->pool().createJob(work);

    for(std::size_t i = 0; i < childJobs; ++i)
    {
#ifdef RAYTRACER_JOBS_EXAMPLE_USE_CLOSURE_JOB
        Job* childJob = worker->pool().createClosureJobAsChild([&engine, secondLevelChildJobs](Job& job)
        {
            for(std::size_t i = 0; i < secondLevelChildJobs; ++i)
            {
                Worker* worker = engine.threadWorker();
                Job* childJob = worker->pool().createJobAsChild(work, &job);

                worker->submit(childJob);
            }

            work(job);
        }, root);
#else
        Job* childJob = worker->pool().createJobAsChild([](Job& job)
        {
            for(std::size_t i = 0; i < 10; ++i)
            {
                Engine* engine = job.getData<Engine*>();
                Worker* worker = engine->threadWorker();

                worker->submit(worker->pool().createJobAsChild(work, &job));
            }

            work(job);
        }, &engine, root);
#endif // RAYTRACER_JOBS_EXAMPLE_USE_CLOSURE_JOB

        worker->submit(childJob);
    }

    worker->submit(root);
    worker->wait(root);

    auto totalTime = std::chrono::high_resolution_clock::now() - start;

    for(const Worker& worker : engine.workers())
    {
        std::cout << "Worker " << worker.threadId() << ": " << worker.totalJobsRun() << " jobs run"
            << " (discarded: " << worker.totalJobsDiscarded() << ", " << worker.pool().jobsFactor()*100
            << "% pool used, max cycles without jobs: " << worker.maxCyclesWithoutJobs() << ")\n";
    }

    std::cout << "\nTotal jobs: " << totalJobs << "\n";
    std::cout << "Total jobs run: " << engine.totalJobsRun() << "\n";
    std::cout << "Total time: " << std::chrono::duration_cast<std::chrono::microseconds>(totalTime).count() / 1000.0f << " ms\n";
}
