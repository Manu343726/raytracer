#ifndef RAYTRACER_JOBS_JOBQUEUE_HPP
#define RAYTRACER_JOBS_JOBQUEUE_HPP

#include "job.hpp"
#include <vector>

namespace ray
{

namespace jobs
{

class JobQueue
{
public:
    JobQueue(std::size_t maxJobs);

    bool push(Job* job);
    Job* pop();
    Job* steal();
    std::size_t size() const;
    bool empty() const;

private:
    std::vector<Job*> _jobs;
    std::atomic<std::size_t> _top, _bottom;
};

}

}

#endif // RAYTRACER_JOBS_JOBQUEUE_HPP
