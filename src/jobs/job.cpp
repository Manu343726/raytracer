#include <raytracer/jobs/job.hpp>

using namespace rt::jobs;

Job::Job(JobFunction jobFunction, Job* parent) :
    _payload{
        jobFunction,
        parent
    }
{
    _payload.unfinishedChildrenJobs.store(1, std::memory_order_seq_cst);

    if(_payload.parent != nullptr)
    {
        _payload.parent->incrementUnfinishedChildrenJobs();
    }
}

bool Job::run()
{
    if(finished())
    {
        return false;
    }

    JobFunction jobFunction = _payload.function;

    if(jobFunction != nullptr)
    {
        jobFunction(*this);


        // When the job is marked as finished, we run the job function
        // again as a callback with teardown work to run when the job is
        // marked as finished. To do so, the user reassigns the job function
        // by calling Job::whenFinished() in the body of the job function
        // (When the job is run). We later check if the job function has changed,
        // and mark a job with the same previous function, that is, a job with no
        // custom whenFinished() callback assigned during run; as having null function
        // Later, Job::finish() executes the function again only if not null
        if(_payload.function == jobFunction)
        {
            _payload.function = nullptr;
        }

        finish();
    }

    return true;
}

bool Job::finished() const
{
    return _payload.unfinishedChildrenJobs.load(std::memory_order_seq_cst) == 0;
}

void Job::incrementUnfinishedChildrenJobs()
{
    _payload.unfinishedChildrenJobs.fetch_add(1, std::memory_order_seq_cst);
}

bool Job::decrementUnfinishedChildrenJobs()
{
    return _payload.unfinishedChildrenJobs.fetch_sub(1, std::memory_order_seq_cst) == 1;
}

std::int32_t Job::unfinishedChildrenJobs() const
{
    return _payload.unfinishedChildrenJobs.load(std::memory_order_seq_cst);
}

Job* Job::parent() const
{
    return _payload.parent;
}

void Job::finish()
{
    if(decrementUnfinishedChildrenJobs())
    {
        if(_payload.function != nullptr)
        {
            // If the job function was left as a whenFinished() callback,
            // execute it
            _payload.function(*this);
        }

        if(_payload.parent != nullptr)
        {
            _payload.parent->finish();
        }
    }
}

void Job::discard()
{
    finish();
}

void* Job::data()
{
    return &_padding[0];
}

void Job::whenFinished(JobFunction jobFunction)
{
    _payload.function = jobFunction;
}

JobFunction Job::function() const
{
    return _payload.function;
}

std::uintptr_t Job::id() const
{
    return reinterpret_cast<std::uintptr_t>(this);
}
