#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "job.h"
#include "factory.h"

class Scheduler
{
public:
    Scheduler(const Jobs& jobs, const Factory& factory);
    virtual ~Scheduler() {}

    virtual void run() = 0;

    const Jobs& get_result() const { return _factory.get_jobs(); }
    unsigned get_cost() const { return _factory.get_cost(); }
    unsigned get_count() const { return _count; }

protected:
    Jobs    _jobs;
    Factory _factory;
    unsigned _count;
};

#endif // SCHEDULER_H
