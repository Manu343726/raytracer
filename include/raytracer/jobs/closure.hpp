#ifndef RAYTRACER_JOBS_CLOSURE_HPP
#define RAYTRACER_JOBS_CLOSURE_HPP

#include "job.hpp"
#include <raytracer/utils.hpp>

namespace rt
{

namespace jobs
{

template<typename Function>
class Closure : public Function
{
public:
    Closure(Function function) :
        Function{std::move(function)}
    {}

    template<typename... Args>
    void run(Job& job, Args&&... args)
    {
        (*this)(job, std::forward<Args>(args)...);
    }
};

template<typename Function>
Job* closure(Job* job, Function function, Job* parent = nullptr)
{
    if(job == nullptr)
    {
        return nullptr;
    }

    // What this function does is to inject a non-pod payload
    // into a previously allocated job. Since jobs are PODs, we must
    // manually destroy the payload after running the job. This is safe
    // as long as there are no other C++ semantics involved (copy, move,
    // etc) which is true for our preallocated array of POD jobs

    auto jobFunction = [](Job& job)
    {
        if constexpr(sizeof(Closure<Function>) <= Job::maxDataSize())
        {
            auto& closure = job.getData<Closure<Function>>();
            closure.run(job);

            // Install a finished callback to destroy the closure
            // when the job is marked as finished. This allows any child job
            // to capture references to the parent closure
            job.whenFinished([](Job& job)
            {
                rt::utils::destroy(job.getData<Closure<Function>>());
            });
        }
        else
        {
            auto& closure = job.getData<std::unique_ptr<Closure<Function>>>();
            closure->run(job);

            // Install a finished callback to destroy the closure
            // when the job is marked as finished. This allows any child job
            // to capture references to the parent closure
            job.whenFinished([](Job& job)
            {
                rt::utils::destroy(job.getData<std::unique_ptr<Closure<Function>>>());
            });
        }
    };

    // Initialize the allocated job:
    rt::utils::construct<Job>(job, jobFunction, parent);

    if constexpr(sizeof(Closure<Function>) <= Job::maxDataSize())
    {
        // Construct the closure in the job payload:
        job->constructData<Closure<Function>>(function);
    }
    else
    {
        static_assert(sizeof(Function) != sizeof(Function));
        // The closure object does not fit in the job payload,
        // dynamically allocate it:
        job->constructData<std::unique_ptr<Closure<Function>>>(std::make_unique<Closure<Function>>(function));
    }

    return job;
}

}

}

#endif // RAYTRACER_JOBS_CLOSURE_HPP
