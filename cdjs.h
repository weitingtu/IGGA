#ifndef CDJS_H
#define CDJS_H

#include "scheduler.h"

class CDJS : public Scheduler
{
public:
    CDJS(const Jobs& jobs, const Factory& factory, const SeqFactory& sf);

    virtual void run();
};

#endif // CDJS_H
