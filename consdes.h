#ifndef CONSDES_H
#define CONSDES_H

#include "job.h"
#include "factory.h"

class ConsDes
{
public:
    ConsDes(unsigned d, const Jobs& jobs, const Factory& Factory);

    void run();
    const Jobs& get_result() const { return _factory.get_jobs(); }

private:
    Jobs _run(Jobs pi1, Jobs pi2);

    unsigned _d;
    Jobs     _jobs;
    Factory  _factory;
};

#endif // CONSDES_H
