#include "job.h"
#include <stdio.h>
#include "factory.h"

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

void print_jobs(const Jobs& jobs, const SeqFactory& sf)
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
    printf("cost %u\n", sf.tct(jobs));
}

void print_jobs(const JobsSeq& jobs, const SeqFactory& sf)
{
    print_jobs(Jobs(jobs.begin(), jobs.end()), sf);
}
