#ifndef FACTORY_H
#define FACTORY_H

#include "job.h"
#include <vector>

class Factory
{
public:
    Factory(unsigned num_of_machine);

    unsigned add_jobs(const std::vector<Job>& jobs);
    void print() const;
    unsigned get_markspan() const { return _machine_times.back(); }
    unsigned get_cost() const;

private:
    unsigned _add_job(const Job& new_job);

    std::vector<unsigned> _machine_times;
    std::vector<Job>      _jobs;
};

#endif // FACTORY_H
