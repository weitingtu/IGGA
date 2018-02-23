#ifndef CFI_H
#define CFI_H

#include "job.h"
#include "factory.h"
#include <set>

class CFI
{
public:
    CFI(const Jobs& jobs, const Factory& factory);

    void run();
    const Jobs& get_result() const { return _jobs; }

private:
    Job _create_job(const Job& job, unsigned job_factor) const;
    Job _create_artificial_job(const std::set<unsigned>& job_set) const;
    unsigned _get_idle_time(const Job& j) const;
    unsigned _get_idle_time(const Job& j1, const Job& j2) const;
    unsigned _get_index_function_value(const Jobs& pi, unsigned job_id);
    Jobs _isa();
    Jobs _neh(Jobs pi);

    Jobs    _jobs;
    Factory _factory;
};

#endif // CFI_H
