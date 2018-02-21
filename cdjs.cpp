#include "cdjs.h"

CDJS::CDJS(const Jobs &jobs, const Factory &factory) :
    _jobs(jobs),
    _factory(factory)
{
}

void CDJS::run()
{
    Jobs pi = _jobs;
    Jobs best = pi;
    _factory.add_jobs(best);
    unsigned min_cost = _factory.get_cost();

    for(size_t i = 0; i < _jobs.size(); ++i)
    {
        for(size_t j = i + 1; j < _jobs.size(); ++j)
        {
            std::swap(pi.at(i), pi.at(j));
            _factory.add_jobs(pi);
            unsigned cost = _factory.get_cost();
            if(cost < min_cost)
            {
                best = pi;
                min_cost = cost;
            }
        }
    }
    _factory.add_jobs(best);
    _factory.print();
}
