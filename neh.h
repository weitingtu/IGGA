#ifndef NEH_H
#define NEH_H

#include "job.h"
#include "factory.h"
#include <vector>

class NEH
{
public:
    NEH(const Jobs& jobs, const Factory& factory);

    void run();
    Jobs run(const Jobs& pi1);
private:
    Jobs    _jobs;
    Factory _factory;
};

#endif // NEH_H
