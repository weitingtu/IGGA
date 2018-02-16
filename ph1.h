#ifndef PH1_H
#define PH1_H

#include "job.h"
#include "factory.h"

class PH1
{
public:
    PH1(const Jobs& jobs, const Factory& Factory);

    void run();
private:
    Jobs _isa();
    Jobs _neh(Jobs pi);
    Jobs _pair_wise_exchange(Jobs pi);

    Jobs    _jobs;
    Factory _factory;
    const unsigned _r_max;
};

#endif // PH1_H
