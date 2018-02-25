#ifndef RIS_H
#define RIS_H

#include "scheduler.h"

class RIS : public Scheduler
{
public:
    RIS(const Jobs& jobs, const Jobs& pi_best, const Factory& factory);

    virtual void run();
private:
    Jobs    _pi_best;
};

#endif // RIS_H
