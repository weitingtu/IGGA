#ifndef CDJS_H
#define CDJS_H

#include "scheduler.h"
#include "job.h"
#include "factory.h"

class CDJS : public Scheduler
{
public:
    CDJS(const Jobs& jobs, const Factory& factory);

    virtual void run();
};

#endif // CDJS_H
