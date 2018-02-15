#ifndef PH1_H
#define PH1_H

#include "job.h"
#include "factory.h"
#include <vector>

class PH1
{
public:
    PH1(const std::vector<Job>& jobs, const Factory& Factory);

    void isa();
private:
    std::vector<Job> _jobs;
    Factory          _factory;
};

#endif // PH1_H
