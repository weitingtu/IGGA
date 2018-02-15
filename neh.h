#ifndef NEH_H
#define NEH_H

#include "job.h"
#include "factory.h"
#include <vector>

class NEH
{
public:
    NEH(const std::vector<Job>& jobs, const Factory& factory);

    void run();
private:
    std::vector<Job> _jobs;
    Factory          _factory;
};

#endif // NEH_H
