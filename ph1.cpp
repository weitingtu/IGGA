#include "ph1.h"
#include "neh.h"
#include <algorithm>
#include <set>

PH1::PH1(const Jobs& jobs, const Factory& factory, const SeqFactory &sf):
    Scheduler(jobs, factory, sf),
    _r_max(1)
{
}

Jobs PH1::_isa()
{
    if(_jobs.empty())
    {
        return Jobs();
    }

    std::set<Job> pi1(_jobs.begin(), _jobs.end());

    Jobs pi2(1, *(pi1.begin()));
    pi1.erase(pi1.begin());

    while(!pi1.empty())
    {
        auto min_ite = pi1.begin();
        unsigned min_cost = std::numeric_limits<unsigned>::max();
        Jobs best;
        for(auto ite = pi1.begin();ite != pi1.end();++ite)
        {
            Jobs tmp = pi2;
            tmp.push_back(*ite);
            unsigned cost = _sf.tct(tmp);
            if(cost < min_cost)
            {
                min_ite = ite;
                min_cost = cost;
                best = tmp;
            }
        }
        pi1.erase(min_ite);
        pi2 = best;
    }

    return pi2;
}

Jobs PH1::_neh(Jobs pi)
{
    Jobs best = pi;
    unsigned min_cost = _sf.tct(pi);

    NEH neh(pi, _factory, _sf);

    unsigned r = 1;
    while(r <= _r_max)
    {
       ++r;
       pi = neh.run( pi );
       unsigned cost = _sf.tct(pi);
       if(cost < min_cost)
       {
           min_cost = cost;
           best = pi;
       }
    }

    return best;
}

Jobs PH1::_pair_wise_exchange(Jobs pi)
{
    unsigned min_cost = _sf.tct(pi);
    for(size_t i = 0; i < pi.size() - 1; ++i)
    {
        std::swap(pi.at(i), pi.at(i + 1));
        unsigned cost = _sf.tct(pi);
        if(cost < min_cost)
        {
            min_cost = cost;
        }
        else
        {
            std::swap(pi.at(i + 1), pi.at(i));
        }
    }
    return pi;
}

void PH1::run()
{
    Jobs pi = _isa();
    pi = _neh(pi);
    pi = _pair_wise_exchange(pi);
    _factory.add_jobs(pi);
    _result_jobs = pi;
}
