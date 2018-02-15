#include "ph1.h"
#include <algorithm>

PH1::PH1(const std::vector<Job> &jobs, const Factory &Factory):
    _jobs(jobs),
    _factory(Factory)
{

}

void PH1::isa()
{
    std::vector<Job> pi1 = _jobs;
    std::sort(pi1.begin(), pi1.end());

    for(size_t i = 0; i < pi1.size(); ++i)
    {

    }
}
