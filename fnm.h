#ifndef FNM_H
#define FNM_H

#include "job.h"
#include "factory.h"

class FNM
{
public:
    FNM(const Jobs& jobs, const Factory& factory);

    void run();

private:
    Jobs _init();
    Jobs _neh(Jobs pi);

    Jobs    _jobs;
    Factory _factory;
};

#endif // FNM_H
