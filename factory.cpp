#include "factory.h"
#include <stdlib.h>
#include <stdio.h>
#include <QtGlobal>

Factory::Factory(unsigned num_of_machine):
    machine_times(num_of_machine)
{

}

unsigned Factory::add_job(const Job& job)
{
    if(job.processing_times.size() != machine_times.size())
    {
        printf("Job processing times size %zu and machine times size %zu mismatch\n",
               job.processing_times.size(),
               machine_times.size());
        Q_ASSERT(0);
        return 0;
    }
    unsigned total_time = machine_times.at(0);
    std::vector<unsigned> job_machine_times;
    for(size_t i = 0; i < job.processing_times.size(); ++i)
    {
        total_time += job.processing_times[i];
        job_machine_times.push_back(total_time);
    }

    for(size_t i = 0; i < job_machine_times.size() - 1;++i)
    {
        if(job_machine_times[i] >= machine_times.at(i + 1))
        {
            continue;
        }
        unsigned shift = machine_times.at(i + 1) - job_machine_times[i];
        std::transform(job_machine_times.begin(), job_machine_times.end(),
                       job_machine_times.begin(), bind2nd(std::plus<unsigned>(), shift));

    }
    for(auto i : job_machine_times)
    {
        printf("%u ", i);
    }
    printf("\n");
    machine_times = job_machine_times;
    return machine_times.back();
}
