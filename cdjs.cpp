#include "cdjs.h"

CDJS::CDJS(const Jobs &jobs, const Factory &factory) :
    _jobs(jobs),
    _factory(factory)
{
}

void CDJS::run()
{
    Jobs best = _jobs;
    _factory.add_jobs(best);
    unsigned min_cost = _factory.get_cost();

    for(size_t i = 0; i < _jobs.size(); i+=2)
    {
        for(size_t j = i + 2; j < _jobs.size() - 1; j+=2)
        {
            Jobs pi = _jobs;
            std::swap(pi.at(i),     pi.at(j));
            std::swap(pi.at(i + 1), pi.at(j +1));
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
