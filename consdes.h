#ifndef CONSDES_H
#define CONSDES_H

#include "scheduler.h"

class ConsDes : public Scheduler
{
public:
    ConsDes(unsigned d, const Jobs& jobs, const Factory& factory, const SeqFactory& sf);

    virtual void run();

private:
    Jobs _run(Jobs pi1, Jobs pi2);

    const unsigned _d;
};

#endif // CONSDES_H
