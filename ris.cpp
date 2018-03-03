#include "ris.h"
#include "ioApi.h"
#include <QtGlobal>
#include "list"

RIS::RIS(const Jobs &jobs, const Jobs &pi_best, const Factory &factory, const SeqFactory&sf)
    : Scheduler(jobs, factory, sf),
    _pi_best(pi_best)
{
}

struct
{
    bool operator()(const Job& a, const Job& b) const
    {
        return a.id < b.id;
    }
} compId;

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

    SeqFactory sf;
    {
        Jobs jobs = _jobs;
        std::sort(jobs.begin(), jobs.end(), compId);
        sf.init(jobs);
    }

    unsigned best_cost = sf.tct(_pi_best);
    Jobs pi_purown = _pi_best;
    JobsSeq pi_new(_jobs.begin(), _jobs.end());

    for(size_t i = 0;i < _pi_best.size(); ++i)
    {
        unsigned job_id = _pi_best.at(i).id;

        Job insert_job;
        size_t insert_job_pos = 0;
        for(auto ite = pi_new.begin(); ite != pi_new.end(); ++ite, ++insert_job_pos)
        {
            const Job& job = *ite;
            if(job.id == job_id)
            {
                insert_job = job;
                pi_new.erase(ite);
                break;
            }
        }

        Q_ASSERT(pi_new.size() == _pi_best.size() - 1);

        bool find = false;
        for(size_t pos = 0; pos <= pi_new.size(); ++pos)
        {
            auto ite = pi_new.begin();
            std::advance(ite, pos);
            ite = pi_new.insert(ite, insert_job);

            unsigned cost = sf.seq_tct(pi_new);

            if(cost < best_cost)
            {
                pi_purown = Jobs(pi_new.begin(), pi_new.end());
                find = true;
                break;
            }
            pi_new.erase(ite);
        }

        if(find)
        {
            break;
        }

        // restore pi_new
        auto ite = pi_new.begin();
        std::advance(ite, insert_job_pos);
        pi_new.insert(ite, insert_job);
    }
    _factory.add_jobs(pi_purown);
    _result_jobs = pi_purown;
}
