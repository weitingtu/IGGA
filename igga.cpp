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
#include <QTime>

class Init {
public:
    void init(const Jobs& jobs, const Factory& factory);
    void get_best(Jobs &best, unsigned &best_cost) const;       // 取得佳解及其目標函數
    void select(Jobs& pi_incumbent, std::vector<Jobs>& others); // 進入解構前所選擇之排序

private:
    std::vector<Jobs>     _job_sets;
    std::vector<unsigned> _costs;
    std::vector<double>   _cp;
};

// 定義data
void Init::init(const Jobs &jobs, const Factory &factory)
{
    _job_sets.clear();
    _costs.clear();

    // 分別執行四種演算法以得到四個排序
    PH1 ph1(jobs, factory);
    ph1.run();

    FNM fnm(jobs, factory);
    fnm.run();

    LS ls(jobs, factory);
    ls.run();

    CFI cfi(jobs, factory);
    cfi.run();

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

    io_debug("ph1 cost %u %.2f - %.2f\n", ph1.get_cost(), 0.0, _cp.at(0));
    io_debug("fnm cost %u %.2f - %.2f\n", fnm.get_cost(), _cp.at(0), _cp.at(1));
    io_debug("ls  cost %u %.2f - %.2f\n", ls.get_cost(),  _cp.at(1), _cp.at(2));
    io_debug("cfi cost %u %.2f - %.2f\n", cfi.get_cost(), _cp.at(2), _cp.at(3));
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
             selected_idx == 0 ? "ph1"
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

IGGA::IGGA(const Jobs &jobs, const Factory &factory, unsigned d, double jp)
    : Scheduler(jobs, factory),
      _d(d),    // 解構所抽出之工件數
      _jp(jp),  // 跳躍機率設定
      _t0(0),
      _alpha(0),
      _gamma(0)
{
}

IGGA::IGGA(const Jobs &jobs, const Factory &factory,
           unsigned d, double jp,
           unsigned t0, unsigned alpha, unsigned gamma)
    : Scheduler(jobs, factory),
      _d(d),    // 解構所抽出之工件數
      _jp(jp),  // 跳躍機率設定
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
    SeqFactory sf;
    sf.init(_jobs);

    Init init;
    init.init(_jobs, _factory);
    Jobs pi_best;
    unsigned pi_best_cost;
    init.get_best(pi_best, pi_best_cost);

    // 終止條件設定
    const unsigned max_count = _jobs.size() * 200;
    const unsigned max_non_improve_count = 100;

    _count = 0;
    unsigned non_improve_count = 0;
    unsigned t = _t0;

    unsigned cdjs_count = 0;
    unsigned ris_count = 0;
    unsigned consdes_time = 0;
    unsigned cdjs_time = 0;
    unsigned ris_time = 0;
    unsigned local_search_time = 0;
    unsigned crossvoer_time = 0;
    while(_count < max_count && non_improve_count < max_non_improve_count)
    {
        io_debug("Iteration %u (non-improve %u) pi best %u\n",
                 (unsigned)_count, (unsigned)non_improve_count, pi_best_cost);
        Jobs pi_incumbent;
        std::vector<Jobs> others;
        init.select(pi_incumbent, others);

        // Construct/Destruct
        ConsDes consdes(_d, pi_incumbent, _factory);
        {
            QTime t;
            t.start();
            consdes.run();
            consdes_time += t.elapsed();
        }
        Jobs pi_new = consdes.get_result();
        unsigned pi_new_cost = consdes.get_cost();
        io_debug("Cons/Des %u -> %u\n", sf.tct(pi_incumbent), pi_new_cost);

        // Local search
        double rf = (double) rand() / (RAND_MAX + 1.0 );
        Scheduler* s = nullptr;
        if(rf < _jp)
        {
            s = new CDJS(pi_new, _factory);
            io_debug("Rf %.2f < %.2f, select CDJS ", rf, _jp);
        }
        else
        {
            s = new RIS(pi_new, pi_best, _factory);
            io_debug("Rf %.2f >= %.2f, select RIS ", rf, _jp);
        }

        {
            QTime t;
            t.start();
            s->run();
            unsigned tt = t.elapsed();
            local_search_time += tt;
            if(rf < _jp)
            {
                cdjs_time += tt;
                ++cdjs_count;
            }
            else
            {
                ris_time += tt;
                ++ris_count;
            }
        }
        Jobs pi_purown = s->get_result();
        unsigned pi_purown_cost = s->get_cost();

        delete s;
        s = nullptr;

        io_debug("pi' %u -> %u\n", pi_new_cost, pi_purown_cost);

        // 局部搜尋完所得排序pi_purown與建構完之排序pi_new做比較
        if(pi_purown_cost < pi_new_cost)
        {
            std::vector<Jobs> r;
            {
                QTime t;
                t.start();
                r = _crossvoer(pi_purown, others);
                crossvoer_time += t.elapsed();
            }
            bool accept = false;

            io_debug("pi best %u ", pi_best_cost);
            io_debug("pi' %u ", pi_purown_cost);
            io_debug("crossover ");
            for(size_t i = 0; i < r.size(); ++i)
            {
                io_debug("(%u) %u ", (unsigned)i, sf.tct(r.at(i)));
            }
            io_debug("\n");

            if(pi_purown_cost < pi_best_cost)
            {
                io_debug("Accept better pi' cost %u -> %u\n", pi_best_cost, pi_purown_cost);
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
                    io_debug("Accept %u crossover cost %u -> %u\n", (unsigned)i, pi_best_cost, cost);
                    pi_best = r.at(i);
                    pi_best_cost = cost;
                    non_improve_count = 0;
                    accept = true;
                }
            }

            if(!accept)
            {
                ++non_improve_count;
                io_debug("Don't accept pi' and corssvoer pi'\n");
            }
        }
        else
        {
            // Try to Accept
            ++non_improve_count;
            if((0 == _gamma && _is_accept(pi_purown_cost, pi_new_cost))
                    || (0 != _gamma && _is_sa_accept(pi_purown_cost, pi_new_cost, t)))
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
        if(_count % _gamma == 0)
        {
            t = _alpha * t;
        }
    }
    printf("count     : %u\n", _count);
    printf("cdjs count: %u\n", cdjs_count);
    printf("ris count : %u\n", ris_count);
    printf("Construc Destruct: %u msec\n", consdes_time);
    printf("Local search     : %u msec\n", local_search_time);
    printf("CDJS             : %u msec\n", cdjs_time);
    printf("RIS              : %u msec\n", ris_time);
    printf("Crossover        : %u msec\n", crossvoer_time);

    _factory.add_jobs(pi_best);
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
    double ap    = exp( -( (double)( pi_purown - pi_new ) / t ) );
    double alpha = (double) rand() / (RAND_MAX + 1.0 );

    return alpha < ap;
}
