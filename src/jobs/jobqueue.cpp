#include <raytracer/jobs/jobqueue.hpp>
#include <cassert>
#include <spdlog/spdlog.h>

using namespace rt::jobs;

JobQueue::JobQueue(std::size_t maxJobs) :
    _jobs{maxJobs, nullptr},
    _top{0},
    _bottom{0}
{}

bool JobQueue::push(Job* job)
{
        assert(job != nullptr && "Tried to push null job to the work queue");

    std::size_t bottom = _bottom.load(std::memory_order_acquire);

    if(bottom < _jobs.size())
    {
        _jobs[bottom % _jobs.size()] = job;

        // Make sure the job is written before publishing the new botton
        std::atomic_thread_fence(std::memory_order_release);

        _bottom.store(bottom + 1, std::memory_order_release);

        return true;
    }
    else
    {
        return false;
    }
}

Job* JobQueue::pop()
{
    std::size_t bottom = _bottom.load(std::memory_order_acquire);

    if(bottom > 0)
    {
        bottom = bottom - 1;
        _bottom.store(bottom, std::memory_order_release);
    }

    std::atomic_thread_fence(std::memory_order_release);

    std::size_t top = _top.load(std::memory_order_acquire);

    if(top <= bottom)
    {
        Job* job = _jobs[bottom % _jobs.size()];
        //assert(job != nullptr && "null job returned from queue");

        if(top == bottom)
        {
            // This is the last item in the queue. It could happen
            // multiple concurrent access "fight" for this last item.
            // The atomic compare+exchange operation ensures this last item
            // is extracted only once

            std::size_t expectedTop = top;
            const std::size_t nextTop = top + 1;
            std::size_t desiredTop = nextTop;

            if(!_top.compare_exchange_strong(expectedTop, desiredTop,
                    std::memory_order_acq_rel))
            {
                // Someone already took the last item, abort
                job = nullptr;
            }

            _bottom.store(nextTop, std::memory_order_release);
        }

        spdlog::trace("JobQueue::pop() returns {} (top: {}, bottom: {})", job->id(), top % _jobs.size(), bottom % _jobs.size());
        return job;
    }
    else
    {
        // Queue already empty
        _bottom.store(top, std::memory_order_release);
        spdlog::trace("JobQueue::pop() failed (queue empty. top: {}, bottom: {})", top, bottom);
        return nullptr;
    }
}

Job* JobQueue::steal()
{
    std::size_t top = _top.load(std::memory_order_acquire);

    // Put a barrier here to make sure bottom is read after reading
    // top
    std::atomic_thread_fence(std::memory_order_acquire);

    std::size_t bottom = _bottom.load(std::memory_order_acquire);

    if(top < bottom)
    {
        Job* job = _jobs[top % _jobs.size()];
        const std::size_t nextTop = top + 1;
        std::size_t desiredTop = nextTop;

        if(!_top.compare_exchange_weak(top, desiredTop,
                std::memory_order_acq_rel))
        {
            // Some concurrent pop()/steal() operation
            // changed the current top
            return nullptr;
        }
        else
        {
            spdlog::trace("JobQueue::steal(this: {}) returns {} (top: {}, bottom: {})", reinterpret_cast<void*>(this), job->id(), top, bottom);
            return job;
        }
    }
    else
    {
        // The queue is empty
        spdlog::trace("JobQueue::steal(this: {}) failed (queue empty. top: {}, bottom: {})", reinterpret_cast<void*>(this), top, bottom);
        return nullptr;
    }
}

std::size_t JobQueue::size() const
{
    return _bottom.load(std::memory_order_seq_cst) - _top.load(std::memory_order_seq_cst);
}

bool JobQueue::empty() const
{
    return size() == 0;
}
