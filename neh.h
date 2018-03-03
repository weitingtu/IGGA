#ifndef NEH_H
#define NEH_H

#include "scheduler.h"

class NEH : public Scheduler
{
public:
    NEH(const Jobs& jobs, const Factory& factory, const SeqFactory&sf);

    virtual void run();
    Jobs run(const Jobs& pi1);
};

#endif // NEH_H
