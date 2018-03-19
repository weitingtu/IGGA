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
#include "neh.h"
#include "ls_random.h"
#include "ls_none.h"
#include "ioApi.h"
#include "possibility.h"
#include <stdlib.h>
#include <QtGlobal>
#include <QTime>

class Init {
public:
    void init(const Jobs& jobs, const Factory& factory, const SeqFactory& sf);
    void init_neh(const Jobs &jobs, const Factory &factory, const SeqFactory &sf);
    void get_best(Jobs &best, unsigned &best_cost) const;       // 取得佳解及其目標函數
    void select(Jobs& pi_incumbent, std::vector<Jobs>& others); // 進入解構前所選擇之排序

private:
    void _calculate_probability();

    std::vector<Jobs>     _job_sets;
    std::vector<unsigned> _costs;
    std::vector<double>   _cp;
};

void Init::_calculate_probability()
{
    // 計算fitness值
    std::vector<double> fits(_costs.size(), 0.0);

    for(size_t i = 0; i < _costs.size();++i)
    {
        fits.at(i) = 1.0 / (double)_costs.at(i);
    }

    double total_fits = std::accumulate(fits.begin(), fits.end(), 0.0);

    _cp.clear();
    _cp.resize(fits.size(), 0.0);
    _cp.at(0) = fits.at(0) / total_fits;
    for(size_t i = 1;i < fits.size(); ++i)
    {
         // 機率計算
        _cp.at(i) = _cp.at(i - 1) + fits.at(i) / total_fits;
    }
}

// 定義data
void Init::init(const Jobs &jobs, const Factory &factory, const SeqFactory &sf)
{
    _job_sets.clear();
    _costs.clear();

    // 分別執行四種演算法以得到四個排序
    QTime t;
    t.start();
    PH1 ph1(jobs, factory, sf);
    ph1.run();
    io_debug("ph1 %u\n", t.restart());

    FNM fnm(jobs, factory, sf);
    fnm.run();
    io_debug("fnm %u\n", t.restart());

    LS ls(jobs, factory, sf);
    ls.run();
    io_debug("ls  %u\n", t.restart());

    CFI cfi(jobs, factory, sf);
    cfi.run();
    io_debug("cfi %u\n", t.elapsed());

    // 將所得排序存起來
    _job_sets.push_back(ph1.get_result());
    _job_sets.push_back(fnm.get_result());
    _job_sets.push_back(ls.get_result());
    _job_sets.push_back(cfi.get_result());
    // 將排序之目標函數計算出來並存起來
    _costs.push_back(ph1.get_cost());
    _costs.push_back(fnm.get_cost());
    _costs.push_back(ls.get_cost());
    _costs.push_back(cfi.get_cost());

    _calculate_probability();

    io_debug("ph1 cost %u %.2f - %.2f\n", ph1.get_cost(), 0.0, _cp.at(0));
    io_debug("fnm cost %u %.2f - %.2f\n", fnm.get_cost(), _cp.at(0), _cp.at(1));
    io_debug("ls  cost %u %.2f - %.2f\n", ls.get_cost(),  _cp.at(1), _cp.at(2));
    io_debug("cfi cost %u %.2f - %.2f\n", cfi.get_cost(), _cp.at(2), _cp.at(3));
}

void Init::init_neh(const Jobs &jobs, const Factory &factory, const SeqFactory &sf)
{
    _job_sets.clear();
    _costs.clear();

    QTime t;
    t.start();

    NEH neh(jobs, factory, sf);
//    Jobs pi = neh.run( jobs);
    neh.run();
//    Jobs pi_best          = neh.get_result();
//    unsigned pi_best_cost = neh.get_cost();
    io_debug("neh %u\n", t.restart());

//    _job_sets.push_back(pi);
//    _costs.push_back(sf.tct(pi));
    _job_sets.push_back(neh.get_result());
    _costs.push_back(neh.get_cost());

    _calculate_probability();

//    io_debug("neh cost %u %.2f - %.2f\n", sf.tct(pi), 0.0, _cp.at(0));
    io_debug("neh cost %u %.2f - %.2f\n", neh.get_cost(), 0.0, _cp.at(0));
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

    best      = _job_sets.at(min_idx); // 將目標函數值最小之排序設為暫存較佳解
    best_cost = _costs.at(min_idx);    // 目標函數更新
}

void Init::select(Jobs& pi_incumbent, std::vector<Jobs>& others)
{
    if(_costs.empty())
    {
        printf("error: cost empty, unable to select\n");
        return;
    }

    // 隨機產生一個機率(0-1)
    double rf = (double) rand() / (RAND_MAX + 1.0 );
    size_t selected_idx = 0;

    for(selected_idx = 0; selected_idx < _cp.size(); ++selected_idx)
    {
        if(rf < _cp.at(selected_idx))
        {
            break;
        }
    }

    // 選出一個排序pi_incumbent進行解構建構
    pi_incumbent = _job_sets.at(selected_idx);

    io_debug("Rf %.2f, select %s(%u) %u\n",
             rf,
             selected_idx == 0 ? "ph1/neh"
             : selected_idx == 1 ? "fnm"
             : selected_idx == 2 ? "ls" : "cfi",
             (unsigned)selected_idx,
             _costs.at(selected_idx));

    others.clear();
    for(size_t i = 0; i < _job_sets.size(); ++i)
    {
        if(i != selected_idx)
        {
            others.push_back(_job_sets.at(i));
        }
    }
}

IGGA::IGGA(const Jobs &jobs, const Factory &factory, const SeqFactory &sf, unsigned d, double jp, unsigned t)
    : Scheduler(jobs, factory, sf),
      _d(d),    // 解構所抽出之工件數
      _jp(jp),  // 跳躍機率設定
      _t(t),
      _t0(0),
      _alpha(0),
      _gamma(0),
      _init_sol(INIT_SOL::MULTIPLE),
      _local_search(LOCAL_SEARCH::CDJS_RIS),
      _temporature(TEMPORATURE::HATAMI)
{
}

IGGA::IGGA(const Jobs &jobs, const Factory &factory, const SeqFactory &sf,
           unsigned d, double jp, unsigned t,
           unsigned t0, unsigned alpha, unsigned gamma)
    : Scheduler(jobs, factory, sf),
      _d(d),    // 解構所抽出之工件數
      _jp(jp),  // 跳躍機率設定
      _t(t),
      _t0(t0),
      _alpha(alpha),
      _gamma(gamma)
{
}

// 若所得排序pi_purown比建構完之排序pi_new還要優異則更新並進行以下雙點交配
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
    if((TEMPORATURE::SA == _temporature) && (0 == _gamma))
    {
        printf("error: run sa with %u gamma\n", _gamma);
        return;
    }

    QTime time;
    time.start();

    Init init;
    if(INIT_SOL::MULTIPLE == _init_sol)
    {
        init.init(_jobs, _factory, _sf);
    }
    else
    {
        init.init_neh(_jobs, _factory, _sf);
    }
    Jobs pi_best;
    unsigned pi_best_cost;
    init.get_best(pi_best, pi_best_cost);

    // 終止條件設定
    _max_runtime = _jobs.size() * _factory.get_machine_size() / 2 * _t;

    _count = 0;
    _non_improve_count = 0;
    _convergence_count = 0;
    unsigned t = _t0;

    _runtime = 0;
    _convergence_time = 0;

    while( _runtime < _max_runtime)
    {
        io_debug("Iteration %u (non-improve %u) pi best %u\n",
                 (unsigned)_count, (unsigned)_non_improve_count, pi_best_cost);
        Jobs pi_incumbent;
        std::vector<Jobs> others;
        init.select(pi_incumbent, others);

        // Construct/Destruct
        ConsDes consdes(_d, pi_incumbent, _factory, _sf);
        consdes.run();

        Jobs pi_new = consdes.get_result();
        unsigned pi_new_cost = consdes.get_cost();
        io_debug("Cons/Des %u -> %u\n", _sf.tct(pi_incumbent), pi_new_cost);

        // Local search
        Scheduler* s = nullptr;
        if(LOCAL_SEARCH::LS == _local_search)
        {
            s = new LSRandom(pi_new, _factory, _sf);
        }
        else if(LOCAL_SEARCH::CDJS == _local_search)
        {
            s = new CDJS(pi_new, _factory, _sf);
        }
        else if(LOCAL_SEARCH::RIS == _local_search)
        {
            s = new RIS(pi_new, pi_best, _factory, _sf);
        }
        else if(LOCAL_SEARCH::CDJS_RIS == _local_search)
        {
            double rf = (double) rand() / (RAND_MAX + 1.0 );
            if(rf < _jp)
            {
                s = new CDJS(pi_new, _factory, _sf);
                io_debug("Rf %.2f < %.2f, select CDJS ", rf, _jp);
            }
            else
            {
                s = new RIS(pi_new, pi_best, _factory, _sf);
                io_debug("Rf %.2f >= %.2f, select RIS ", rf, _jp);
            }
        }
        else if(LOCAL_SEARCH::NONE == _local_search)
        {
            s = new LSNone(pi_new, _factory, _sf);
        }
        else
        {
            Q_ASSERT(0);
        }

        s->run();
        Jobs pi_purown = s->get_result();
        unsigned pi_purown_cost = s->get_cost();

        delete s;
        s = nullptr;

        io_debug("pi' %u -> %u\n", pi_new_cost, pi_purown_cost);

        // 局部搜尋完所得排序pi_purown與建構完之排序pi_new做比較
        bool accept = false;
        if(((LOCAL_SEARCH::NONE == _local_search) && (pi_purown_cost < pi_best_cost))
                || pi_purown_cost < pi_new_cost)
        {
            std::vector<Jobs> r = _crossvoer(pi_purown, others);
            io_debug("pi best %u ", pi_best_cost);
            io_debug("pi' %u ",     pi_purown_cost);
            if(!r.empty())
            {
                io_debug("crossover ");
                for(size_t i = 0; i < r.size(); ++i)
                {
                    io_debug("(%u) %u ", (unsigned)i, _sf.tct(r.at(i)));
                }
                io_debug("\n");
            }

            if(pi_purown_cost < pi_best_cost)
            {
                io_debug("Accept better pi' cost %u -> %u\n", pi_best_cost, pi_purown_cost);
                pi_best = pi_purown;
                pi_best_cost = pi_purown_cost;
                _non_improve_count = 0;
                accept = true;
            }

            for(size_t i = 0; i < r.size(); ++i)
            {
                unsigned cost = _sf.tct(r.at(i));
                if(cost < pi_best_cost)
                {
                    io_debug("Accept %u crossover cost %u -> %u\n", (unsigned)i, pi_best_cost, cost);
                    pi_best = r.at(i);
                    pi_best_cost = cost;
                    _non_improve_count = 0;
                    accept = true;
                }
            }

            if(!accept)
            {
                ++_non_improve_count;
                io_debug("Don't accept pi' and corssvoer pi'\n");
            }
        }
        else
        {
            // Try to Accept
            ++_non_improve_count;
            if(((TEMPORATURE::HATAMI == _temporature) && _is_accept(pi_purown_cost, pi_new_cost))
                    || ((TEMPORATURE::SA == _temporature) && _is_sa_accept(pi_purown_cost, pi_new_cost, t)))
            {
                io_debug("Accept bad pi' cost %u -> %u\n", pi_best_cost, pi_purown_cost);
                pi_best = pi_purown;
                pi_best_cost = pi_purown_cost;
            }
            else
            {
                io_debug("Don't accept bad pi'\n");
            }
        }
        ++_count;
        if(TEMPORATURE::SA == _temporature&& _count % _gamma == 0)
        {
            t = _alpha * t;
        }
        _runtime +=time.restart();
        if(accept)
        {
            _convergence_time = _runtime;
            _convergence_count = _count;
        }
    }

    _factory.add_jobs(pi_best);
    _result_jobs = pi_best;
    _runtime += time.restart();

    printf("summary\n");
    printf("  count             : %u\n", _count);
    printf("  non-improve count : %u\n", _non_improve_count);
    printf("  convergence count : %u\n", _convergence_count);
    printf("  run time          : %d ms\n", _runtime);
    printf("  convergence time  : %d ms\n", _convergence_time);
    printf("  max run time      : %d ms\n", _max_runtime);
    printf("\n");
}

// 降溫機制
bool IGGA::_is_accept(unsigned pi_purown, unsigned pi_new) const
{
    double ap    = exp( -( ( pi_purown - pi_new ) / pi_new ) * 100 );
    double alpha = (double) rand() / (RAND_MAX + 1.0 );

    return ap < alpha;
}

bool IGGA::_is_sa_accept(unsigned pi_purown, unsigned pi_new, unsigned t) const
{
    double ap    = exp( -( ((double)( pi_purown - pi_new )) / t ) );
    double alpha = (double) rand() / (RAND_MAX + 1.0 );

    return alpha < ap;
}
