#ifndef CONSDES_H
#define CONSDES_H

#include "scheduler.h"
#include "job.h"
#include "factory.h"

class ConsDes : public Scheduler
{
public:
    ConsDes(unsigned d, const Jobs& jobs, const Factory& factory);

    virtual void run();
//    const Jobs& get_result() const { return _factory.get_jobs(); }

private:
    Jobs _run(Jobs pi1, Jobs pi2);

    unsigned _d;
//    Jobs     _jobs;
//    Factory  _factory;
};

#endif // CONSDES_H
