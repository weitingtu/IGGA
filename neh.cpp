#include "neh.h"
#include "utils.h"
#include <algorithm>

NEH::NEH(const Jobs &jobs, const Factory &factory, const SeqFactory &sf)
    : Scheduler(jobs, factory, sf)
{
}

void NEH::run()
{
    Jobs pi1 = _jobs;
    std::sort(pi1.begin(), pi1.end(), greater());

    Jobs pi = run(pi1);
    _factory.add_jobs(pi);
    _result_jobs = pi;
}

Jobs NEH::run(const Jobs& pi1)
{
    Jobs pi2;
    for(size_t i = 0; i < pi1.size(); ++i)
    {
        Jobs best;
        unsigned min_cost = std::numeric_limits<unsigned>::max();
        for(size_t pos = 0; ;++pos)
        {
            auto ite = pi2.begin() + pos;
            ite = pi2.insert(ite, pi1[i]);

            unsigned cost = _sf.tct(pi2);

            if(cost < min_cost)
            {
                min_cost = cost;
                best = pi2;
            }

            pi2.erase(ite);

            if(pos == pi2.size())
            {
                break;
            }
        }
        pi2 = best;
    }
    return pi2;
}
