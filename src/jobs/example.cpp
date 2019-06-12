#include <raytracer/jobs/engine.hpp>
#include <cstdio>
#include <iostream>
#include <thread>
#include <raytracer/math.hpp>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

using namespace ray::jobs;

std::atomic_size_t jobs_run{0};

void work(Job& job)
{
#ifdef RAYTRACER_JOBS_EXAMPLE_SIMULATE_WORK
    spdlog::info("Job {} running on thread {}",
        reinterpret_cast<void*>(&job),
        std::this_thread::get_id());

    auto start = std::chrono::steady_clock::now();
    int i = 0;

    while(std::chrono::steady_clock::now() - start < std::chrono::seconds(1))
    {
        if(i < ray::random(-1.0f, 1.0f) * i)
        {
            i += 1;
        }
    }

    Job* parent = job.parent();

    if(parent != nullptr)
    {
        spdlog::info("Job {} finished (parent unfinished jobs: {})",
            reinterpret_cast<void*>(&job),
            parent->unfinishedChildrenJobs());
    }
    else
    {
        spdlog::info("Job {} finished",
            reinterpret_cast<void*>(&job));
    }
#ifdef RAYTRACER_JOBS_EXAMPLE_LOG_WORK
    std::printf("job %p done\n", reinterpret_cast<void*>(&job));
#endif // RAYTRACER_JOBS_EXAMPLE_LOG_WORK

#endif // RAYTRACER_JOBS_EXAMPLE_SIMULATE_WORK

    jobs_run++;
}

int main(int argc, char** argv)
{
    const std::size_t threads = std::thread::hardware_concurrency();
    const std::size_t childJobs = (argc > 1 ? std::atoi(argv[1]) : 65*100);
    const std::size_t secondLevelChildJobs = (argc > 2 ? std::atoi(argv[2]) : 10);
    const std::size_t totalJobs = 1 + childJobs + (childJobs * secondLevelChildJobs);

    if(argc > 3)
    {
        if(!std::strcmp(argv[3], "trace"))
        {
            spdlog::set_level(spdlog::level::trace);
        }
        else if(!std::strcmp(argv[3], "debug"))
        {
            spdlog::set_level(spdlog::level::debug);
        }
    }

    Engine engine{threads, totalJobs};
    Worker* worker = engine.threadWorker();

    auto start = std::chrono::high_resolution_clock::now();

    Job* root = worker->pool().createJob(work);

    for(std::size_t i = 0; i < childJobs; ++i) // n
    {
        Job* childJob = worker->pool().createClosureJobAsChild([&](Job& childJob)
        {
            auto* worker = engine.threadWorker();

            for(std::size_t i = 0; i < secondLevelChildJobs; ++i)
            {
                Job* childOfChild = worker->pool().createJobAsChild(work, &childJob);
                worker->submit(childOfChild);
            }

            work(childJob);
        }, root);
        worker->submit(childJob);
    }

    worker->submit(root);
    worker->wait(root);

    auto totalTime = std::chrono::high_resolution_clock::now() - start;

    for(const Worker& worker : engine.workers())
    {
        std::cout << "Worker " << worker.threadId() << ": " << worker.totalJobsRun() << " jobs run"
            << " (discarded: " << worker.totalJobsDiscarded() << ", allocated: " << worker.pool().jobs()
            << ", " << worker.pool().jobsFactor()*100
            << "% pool used, max cycles without jobs: " << worker.maxCyclesWithoutJobs() << ")\n";
    }

    std::cout << "\nTotal jobs: " << totalJobs << "\n";
    std::cout << "Total jobs allocated: " << engine.totalJobsAllocated() << "\n";
    std::cout << "Total jobs run: " << engine.totalJobsRun() << " (" << jobs_run << ")\n";
    std::cout << "Total time: " << std::chrono::duration_cast<std::chrono::microseconds>(totalTime).count() / 1000.0f << " ms\n";
}
