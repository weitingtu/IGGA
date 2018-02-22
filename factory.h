#ifndef FACTORY_H
#define FACTORY_H

#include "job.h"
#include <vector>

class Factory
{
public:
    explicit Factory(unsigned num_of_machine);

    unsigned add_jobs(const Jobs& jobs);
    void print() const;
    unsigned get_markspan() const { return _machine_times.back(); }
    unsigned get_cost() const;
    size_t get_machine_size() const { return _machine_times.size(); }

    const Jobs& get_jobs() const { return _jobs; }

private:
    unsigned _add_job(const Job& new_job);

    std::vector<unsigned> _machine_times;
    Jobs                  _jobs;
};

#endif // FACTORY_H
