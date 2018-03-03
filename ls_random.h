#ifndef LSRANDOM_H
#define LSRANDOM_H

#include "scheduler.h"

class LSRandom : public Scheduler
{
public:
    LSRandom(const Jobs& jobs, const Factory&factory, const SeqFactory& sf);

    virtual void run();
};

#endif // LSRANDOM_H
