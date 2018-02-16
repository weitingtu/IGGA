#include "factory.h"
#include <stdlib.h>
#include <stdio.h>
#include <QtGlobal>

Factory::Factory(unsigned num_of_machine):
    _machine_times(num_of_machine, 0)
{
}

void Factory::print() const
{
    for(const Job& job : _jobs)
    {
        printf("Job %u ", job.id);
        for(auto i : job.machine_times)
        {
            printf("%u ", i);
        }
        printf("\n");
    }
}

unsigned Factory::_add_job(const Job& new_job)
{
    if(new_job.processing_times.size() != _machine_times.size())
    {
        printf("Job processing times size %zu and machine times size %zu mismatch\n",
               new_job.processing_times.size(),
               _machine_times.size());
        Q_ASSERT(0);
        return 0;
    }

    _jobs.push_back(new_job);
    Job& job = _jobs.back();
    job.machine_times.clear();

    unsigned total_time = _machine_times.at(0);
    for(size_t i = 0; i < job.processing_times.size(); ++i)
    {
        total_time += job.processing_times[i];
        job.machine_times.push_back(total_time);
    }

    for(size_t i = 0; i < job.machine_times.size() - 1;++i)
    {
        if(job.machine_times[i] >= _machine_times.at(i + 1))
        {
            continue;
        }
        unsigned shift = _machine_times.at(i + 1) - job.machine_times[i];
        std::transform(job.machine_times.begin(), job.machine_times.end(),
                       job.machine_times.begin(), bind2nd(std::plus<unsigned>(), shift));

    }
    _machine_times = job.machine_times;
    return _machine_times.back();
}

unsigned Factory::add_jobs(const std::vector<Job>& jobs)
{
    _jobs.clear();
    std::fill( _machine_times.begin(), _machine_times.end(), 0);

    unsigned cost = 0;
    for(size_t i = 0; i < jobs.size(); ++i)
    {
        cost += _add_job(jobs[i]);
    }
    return cost;
}

unsigned Factory::get_cost() const
{
    unsigned cost = 0;
    for(size_t i = 0; i < _jobs.size(); ++i)
    {
        cost += _jobs[i].machine_times.back();
    }
    return cost;
}
