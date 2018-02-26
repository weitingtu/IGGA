#define _USE_MATH_DEFINES
#include <math.h>
#include "igga.h"
#include "ph1.h"
#include "fnm.h"
#include "ls.h"
#include "cfi.h"
#include "consdes.h"
#include "ris.h"
#include "cdjs.h"
#include "ioApi.h"
#include "possibility.h"
#include <stdlib.h>
#include <QtGlobal>

class Init {
public:
    void init(const Jobs& jobs, const Factory& factory);
    void get_best(Jobs &best, unsigned &best_cost) const;
    void select(Jobs& pi_incumbent, std::vector<Jobs>& others);

private:
    std::vector<Jobs>     _job_sets;
    std::vector<unsigned> _costs;
    std::vector<double>   _cp;
};

void Init::init(const Jobs &jobs, const Factory &factory)
{
    _job_sets.clear();
    _costs.clear();

    PH1 ph1(jobs, factory);
    ph1.run();

    FNM fnm(jobs, factory);
    fnm.run();

    LS ls(jobs, factory);
    ls.run();

    CFI cfi(jobs, factory);
    cfi.run();

    _job_sets.push_back(ph1.get_result());
    _job_sets.push_back(fnm.get_result());
    _job_sets.push_back(ls.get_result());
    _job_sets.push_back(cfi.get_result());
    _costs.push_back(ph1.get_cost());
    _costs.push_back(fnm.get_cost());
    _costs.push_back(ls.get_cost());
    _costs.push_back(cfi.get_cost());

    std::vector<double> fits(_costs.size(), 0.0);

    for(size_t i = 0; i < _costs.size();++i)
    {
        fits.at(i) = 1.0 / (double)_costs.at(i);
    }

    double total_fits = std::accumulate(fits.begin(), fits.end(), 0);

    _cp.clear();
    _cp.resize(fits.size(), 0.0);
    _cp.at(0) = fits.at(0) / total_fits;
    for(size_t i = 1;i < fits.size(); ++i)
    {
        _cp.at(i) = _cp.at(i - 1) + fits.at(i) / total_fits;
    }
}

void Init::get_best(Jobs& best, unsigned& best_cost) const
{
    best.clear();
    best_cost = 0;

    if(_costs.empty())
    {
        printf("error: cost empty, unable to get best jobs\n");
        return;
    }
    unsigned min_cost = _costs.at(0);
    size_t   min_idx  = 0;

    for(size_t i = 1; i < _costs.size(); ++i)
    {
        if(_costs.at(i) < min_cost )
        {
            min_cost = _costs.at(i);
            min_idx = i;
        }
    }

    best      = _job_sets.at(min_idx);
    best_cost = _costs.at(min_idx);
}

void Init::select(Jobs& pi_incumbent, std::vector<Jobs>& others)
{
    if(_costs.empty())
    {
        printf("error: cost empty, unable to select\n");
        return;
    }

    double rf = (double) rand() / (RAND_MAX + 1.0 );
    size_t selected_idx = 0;

    for(selected_idx = 0; selected_idx < _cp.size(); ++selected_idx)
    {
        if(rf < _cp.at(selected_idx))
        {
            break;
        }
    }
    io_debug("random possibility %f, selected index %u\n", rf, (unsigned)selected_idx);

    pi_incumbent = _job_sets.at(selected_idx);

    others.clear();
    for(size_t i = 0; i < _job_sets.size(); ++i)
    {
        if(i != selected_idx)
        {
            others.push_back(_job_sets.at(i));
        }
    }
}

IGGA::IGGA(const Jobs &jobs, const Factory &factory, unsigned d, double jp)
    : Scheduler(jobs, factory),
      _d(d),
      _jp(jp)
{
}

Jobs IGGA::_crossvoer(const Jobs& pi_new, const Jobs& jobs)
{
    std::vector<int> indice = Possibility::KnuthShuffle(2, 0, jobs.size() - 1);
    unsigned idx1 = std::min(indice.front(), indice.back());
    unsigned idx2 = std::max(indice.front(), indice.back());

    Jobs sub_pi_new(pi_new.begin() + idx1, pi_new.begin() + idx2 + 1);
    std::set<unsigned> sub_pi_new_set;
    for(size_t i = 0; i < sub_pi_new.size(); ++i)
    {
        sub_pi_new_set.insert(sub_pi_new.at(i).id);
    }

    Jobs sub_jobs;
    for(size_t i = 0; i < jobs.size(); ++i)
    {
        const Job& job = jobs.at(i);
        if(sub_pi_new_set.find(job.id) == sub_pi_new_set.end())
        {
            sub_jobs.push_back(job);
        }
    }
    Q_ASSERT(sub_jobs.size() + sub_pi_new.size() == jobs.size());

    Jobs pi(sub_jobs.begin(), sub_jobs.begin() + idx1);
    pi.insert(pi.end(), sub_pi_new.begin(), sub_pi_new.end());
    pi.insert(pi.end(), sub_jobs.begin() + idx1, sub_jobs.end());

    Q_ASSERT(pi.size() == jobs.size());

    _factory.add_jobs(pi);
    _factory.print();

    return pi;
}

std::vector<Jobs> IGGA::_crossvoer(const Jobs& pi_new, const std::vector<Jobs>& others)
{
    std::vector<Jobs> r;
    for(size_t i = 0; i < others.size(); ++i)
    {
        r.push_back(_crossvoer(pi_new, others.at(i)));
    }

    return r;
}

void IGGA::run()
{
    Init init;
    init.init(_jobs, _factory);
    Jobs pi_best;
    unsigned pi_best_cost;
    init.get_best(pi_best, pi_best_cost);

    const unsigned max_count = _jobs.size() * 200;
    const unsigned max_non_improve_count = 100;

    unsigned count = 0;
    unsigned non_improve_count = 0;
    while(count < max_count && non_improve_count < max_non_improve_count)
    {
        Jobs pi_incumbent;
        std::vector<Jobs> others;
        init.select(pi_incumbent, others);

        // Construct/Destruct
        ConsDes consdes(_d, pi_incumbent, _factory);
        consdes.run();
        Jobs pi_new = consdes.get_result();
        unsigned pi_new_cost = consdes.get_cost();

        // Local search
        double rf = (double) rand() / (RAND_MAX + 1.0 );
        Scheduler* s = nullptr;
//        printf("rf %f jp %f\n", rf, _jp);
        if(rf < _jp)
        {
            s = new CDJS(pi_new, _factory);
        }
        else
        {
            s = new RIS(pi_new, pi_best, _factory);
        }

        s->run();
        Jobs pi_purown = s->get_result();
        unsigned pi_purown_cost = s->get_cost();
//        printf("count %zu pi purown %u pi best %u pi new %u\n", count, pi_purown_cost, pi_best_cost, pi_new_cost);

        delete s;
        s = nullptr;

        if(pi_purown_cost < pi_new_cost)
        {
            std::vector<Jobs> r = _crossvoer(pi_purown, others);
            bool accept = false;

            if(pi_purown_cost < pi_best_cost)
            {
                pi_best = pi_purown;
                pi_best_cost = pi_purown_cost;
                non_improve_count = 0;
                accept = true;
            }

            for(size_t i = 0; i < r.size(); ++i)
            {
                _factory.add_jobs(r.at(i));
                unsigned cost = _factory.get_cost();
                if(cost < pi_best_cost)
                {
                    pi_best = r.at(i);
                    pi_best_cost = cost;
                    non_improve_count = 0;
                    accept = true;
                }
            }
            if(!accept)
            {
                ++non_improve_count;
            }
        }
        else
        {
            // Try to Accept
            ++non_improve_count;
            if(_is_accept(pi_purown_cost, pi_new_cost))
            {
                pi_best = pi_purown;
                pi_best_cost = pi_purown_cost;
            }
        }
        ++count;
    }

    _factory.add_jobs(pi_best);
}

bool IGGA::_is_accept(unsigned pi_purown, unsigned pi_new) const
{
    double ap    = exp( -( ( pi_purown - pi_new ) / pi_new ) * 100 );
    double alpha = (double) rand() / (RAND_MAX + 1.0 );

    return ap < alpha;
}
