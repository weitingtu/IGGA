#include "ris.h"
#include "ioApi.h"
#include <QtGlobal>

RIS::RIS(const Jobs &jobs, const Jobs &pi_best, const Factory &factory)
    : Scheduler(jobs, factory),
    _pi_best(pi_best)
{
}

void RIS::run()
{
    if(_jobs.empty())
    {
        printf("error: pi new is empty\n");
        return;
    }
    if(_pi_best.empty())
    {
        printf("error: pi best is empty\n");
        return;
    }
    if(_jobs.size() != _pi_best.size())
    {
        printf("error: pi size %zu != pi best size %zu\n", _jobs.size(), _pi_best.size());
        return;
    }

    _factory.add_jobs(_pi_best);
    unsigned best_cost = _factory.get_cost();
    Jobs pi_purown = _pi_best;

    for(size_t i = 0;i < _pi_best.size(); ++i)
    {
        unsigned job_id = _pi_best.at(i).id;

        Job insert_job;
        Jobs pi_new;
        for(size_t j = 0; j < _jobs.size(); ++j)
        {
            const Job& job = _jobs.at(j);
            if(job.id == job_id)
            {
                insert_job = job;
            }
            else
            {
                pi_new.push_back(job);
            }
        }

        Q_ASSERT(pi_new.size() == _pi_best.size() - 1);

        bool find = false;
        for(size_t pos = 0; ;++pos)
        {
            Jobs tmp = pi_new;
            auto ite = tmp.begin() + pos;
            tmp.insert(ite, insert_job);

            _factory.add_jobs(tmp);
            unsigned cost = _factory.get_cost();

            if(cost < best_cost)
            {
                pi_purown = tmp;
                find = true;
                break;
            }

            if(pos == pi_new.size())
            {
                break;
            }
        }
        if(find)
        {
            break;
        }
    }

    _factory.add_jobs(pi_purown);
    _factory.print();
}
