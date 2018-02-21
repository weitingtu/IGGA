#ifndef RIS_H
#define RIS_H

#include "job.h"
#include "factory.h"

class RIS
{
public:
    RIS(const Jobs& jobs, const Jobs& pi_best, const Factory& Factory);

    void run();
private:
    Jobs    _jobs;
    Jobs    _pi_best;
    Factory _factory;
};

#endif // RIS_H
