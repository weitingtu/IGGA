#include "ls_random.h"
#include "possibility.h"
#include <QtGlobal>

LSRandom::LSRandom(const Jobs &jobs, const Factory &factory, const SeqFactory &sf):
    Scheduler(jobs, factory, sf)
{

}

void LSRandom::run()
{
    std::vector<int> indice = Possibility::KnuthShuffle( _jobs.size(), 0, _jobs.size() - 1);
    Q_ASSERT(indice.size() == _jobs.size());

    JobsSeq pi(_jobs.begin(), _jobs.end());
    unsigned init_cost = _sf.seq_tct(pi);

    for(size_t i = 0; i < indice.size(); ++i)
    {
        int idx = indice.at(i);

        auto ite = pi.begin();
        std::advance(ite, idx);
        Job job = *ite;
        pi.erase(ite);

        for(size_t j = 0; j <= pi.size(); ++j)
        {
            ite = pi.begin();
            std::advance(ite, j);
            ite = pi.insert(ite, job);
            unsigned cost = _sf.seq_tct(pi);
            if(cost < init_cost)
            {
                _factory.add_jobs(Jobs(pi.begin(), pi.end()));
                return;
            }
            pi.erase(ite);
        }

        ite = pi.begin();
        std::advance(ite, idx);
        pi.insert(ite, job);
    }

    _factory.add_jobs(_jobs);
}
