#include "job.h"
#include <stdio.h>

Job::Job():
    id(0),
    processing_times(),
    machine_times(),
    _sum(0)
{
}

Job::Job(size_t m):
    id(0),
    processing_times(m, 0),
    machine_times(m, 0),
    _sum(0)
{
}

unsigned Job::get_sum() const
{
    if(0 == _sum)
    {
        _sum = std::accumulate(processing_times.begin(),processing_times.end(),0);
    }
    return _sum;
}

void print_jobs(const Jobs& jobs)
{
    for(const Job& job : jobs)
    {
        printf("Job %u ", job.id);
        for(auto i : job.machine_times)
        {
            printf("%u ", i);
        }
        printf("\n");
    }
}
