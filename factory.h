#ifndef FACTORY_H
#define FACTORY_H

#include "job.h"
#include <vector>

class Factory
{
public:
    Factory(unsigned num_of_machine);

    unsigned add_job(const Job& job);

    std::vector<unsigned> machine_times;
};

#endif // FACTORY_H
