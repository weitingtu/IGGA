#ifndef CDJS_H
#define CDJS_H

#include "job.h"
#include "factory.h"

class CDJS
{
public:
    CDJS(const Jobs& jobs, const Factory& factory);

    void run();

private:
    Jobs    _jobs;
    Factory _factory;
};

#endif // CDJS_H
