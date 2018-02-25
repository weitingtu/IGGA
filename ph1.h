#ifndef PH1_H
#define PH1_H

#include "scheduler.h"

class PH1 : public Scheduler
{
public:
    PH1(const Jobs& jobs, const Factory& factory);

    virtual void run();
private:
    Jobs _isa();
    Jobs _neh(Jobs pi);
    Jobs _pair_wise_exchange(Jobs pi);

    const unsigned _r_max;
};

#endif // PH1_H
