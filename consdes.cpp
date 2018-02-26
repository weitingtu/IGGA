#include "consdes.h"
#include "possibility.h"
#include <set>

ConsDes::ConsDes(unsigned d, const Jobs& jobs, const Factory& factory)
    : Scheduler(jobs, factory), _d(d)
{
}

void ConsDes::run()
{
    if(_d > _jobs.size())
    {
        printf("error: d %d > jobs size %zu\n", _d, _jobs.size());
        return;
    }

    std::vector<int> id_vec = Possibility::KnuthShuffle( _d, 0, _jobs.size() - 1);
    std::set<unsigned> selected_ids(id_vec.begin(), id_vec.end());

    Jobs pi1;
    Jobs pi2;

    for(size_t i = 0; i < _jobs.size(); ++i)
    {
        const Job& job = _jobs.at(i);
        if(selected_ids.find(job.id) != selected_ids.end())
        {
            pi1.push_back(job);
        }
        else
        {
            pi2.push_back(job);
        }
    }

    _run(pi1, pi2);
}

Jobs ConsDes::_run(Jobs pi1, Jobs pi2)
{
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
    _factory.add_jobs(pi2);
//    _factory.print();

    return pi2;
}
