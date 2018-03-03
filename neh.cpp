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
            Jobs tmp = pi2;
            auto ite = tmp.begin() + pos;
            tmp.insert(ite, pi1[i]);

            _factory.add_jobs(tmp);
            unsigned cost = _factory.get_cost();

            if(cost < min_cost)
            {
                min_cost = cost;
                best = tmp;
            }

            if(pos == pi2.size())
            {
                break;
            }
        }
        pi2 = best;
    }
    return _factory.get_jobs();
}
