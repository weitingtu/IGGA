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

class SeqFactory
{
public:
    SeqFactory(): _d_matrix() {}

    void init(const Jobs& jobs);
    void print() const;
    unsigned tct(const Jobs& jobs) const;
    unsigned seq_tct(const JobsSeq& jobs) const;

private:
    unsigned _d(const Job& ji, const Job& jj) const;

    std::vector<std::vector<unsigned > > _d_matrix;

};

#endif // FACTORY_H
