#ifndef LS_NONE_H
#define LS_NONE_H

#include "scheduler.h"

class LSNone : public Scheduler
{
public:
    LSNone(const Jobs& jobs, const Factory&factory, const SeqFactory& sf);

    virtual void run();
};

#endif // LS_NONE_H
