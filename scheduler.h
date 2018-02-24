#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "job.h"
#include "factory.h"

class Scheduler
{
public:
    Scheduler(const Jobs& jobs, const Factory& Factory);
    virtual ~Scheduler() {}

    virtual void run() = 0;

    const Jobs& get_result() const { return _factory.get_jobs(); }

protected:
    Jobs    _jobs;
    Factory _factory;
};

#endif // SCHEDULER_H
