#ifndef RIS_H
#define RIS_H

#include "scheduler.h"

class RIS : public Scheduler
{
public:
    RIS(const Jobs& jobs, const Jobs& pi_best, const Factory& factory, const SeqFactory& sf);

    virtual void run();
    void run2();
private:
    Jobs    _pi_best;
};

#endif // RIS_H
