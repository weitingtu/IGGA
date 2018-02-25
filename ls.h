#ifndef LS_H
#define LS_H

#include "scheduler.h"

class LS : public Scheduler
{
public:
    LS(const Jobs& jobs, const Factory& factory);

    virtual void run();
};

#endif // LS_H
