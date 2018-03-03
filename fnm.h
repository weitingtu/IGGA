#ifndef FNM_H
#define FNM_H

#include "scheduler.h"

class FNM : public Scheduler
{
public:
    FNM(const Jobs& jobs, const Factory& factory, const SeqFactory& sf);

    virtual void run();

private:
    Jobs _init();
    Jobs _neh(Jobs pi);
};

#endif // FNM_H
