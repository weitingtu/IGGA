#include "ph1.h"
#include "neh.h"
#include <algorithm>
#include <set>

PH1::PH1(const Jobs &jobs, const Factory &Factory):
    _jobs(jobs),
    _factory(Factory),
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
    _factory.add_jobs(pi2);
    _factory.print();

    while(!pi1.empty())
    {
        auto min_ite = pi1.begin();
        unsigned min_cost = std::numeric_limits<unsigned>::max();
        Jobs best;
        for(auto ite = pi1.begin();ite != pi1.end();++ite)
        {
            Jobs tmp = pi2;
            tmp.push_back(*ite);
            _factory.add_jobs(tmp);
            unsigned cost = _factory.get_cost();
            if(cost < min_cost)
            {
                min_cost = cost;
                best = tmp;
            }
        }
        pi1.erase(min_ite);
        pi2 = best;
        _factory.add_jobs(pi2);
        _factory.print();
    }
    _factory.add_jobs(pi2);
    _factory.print();

    return pi2;
}

Jobs PH1::_neh(Jobs pi)
{
    _factory.add_jobs(pi);
    _factory.print();

    Jobs best = pi;
    unsigned min_cost = _factory.get_cost();

    NEH neh(pi, _factory);

    unsigned r = 1;
    while(r <= _r_max)
    {
       ++r;
       pi = neh.run( pi );
       _factory.add_jobs( pi );
       unsigned cost = _factory.get_cost();
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
    _factory.add_jobs(pi);
    unsigned min_cost = _factory.get_cost();
    for(size_t i = 0; i < pi.size() - 1; ++i)
    {
        Jobs tmp = pi;
        std::swap(tmp.at(i), tmp.at(i + 1));
        _factory.add_jobs(tmp);
        unsigned cost = _factory.get_cost();
        if(cost < min_cost)
        {
            pi = tmp;
            min_cost = cost;
        }
    }
    return pi;
}

void PH1::run()
{
    Jobs pi = _isa();
    pi = _neh(pi);
    pi = _pair_wise_exchange(pi);
}
