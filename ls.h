#ifndef LS_H
#define LS_H

#include "job.h"
#include "factory.h"

class LS
{
public:
    LS(const Jobs& jobs, const Factory& Factory);

    void run();

private:
    Jobs    _jobs;
    Factory _factory;
};

#endif // LS_H
