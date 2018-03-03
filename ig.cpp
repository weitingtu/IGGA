#include "ig.h"
#include "neh.h"
#include "consdes.h"
#include "ls_random.h"
#include "possibility.h"
#include <stdlib.h>
#include <QTime>

IG::IG(const Jobs& jobs, const Factory& factory, const SeqFactory &sf, unsigned d, unsigned t, unsigned t0, double alpha, unsigned gamma)
    : Scheduler(jobs, factory, sf),
      _d(d),
      _t(t),
      _t0(t0),
      _alpha(alpha),
      _gamma(gamma),
      _ls(false)
{
}

IG::IG(const Jobs& jobs, const Factory& factory, const SeqFactory &sf, unsigned d, unsigned t, unsigned t0, double alpha, unsigned gamma, bool ls)
    : Scheduler(jobs, factory, sf),
      _d(d),
      _t(t),
      _t0(t0),
      _alpha(alpha),
      _gamma(gamma),
      _ls(ls)
{
}

void IG::run()
{
    QTime time;
    time.start();

    NEH neh(_jobs, _factory, _sf);
    neh.run();
    Jobs pi_best          = neh.get_result();
    unsigned pi_best_cost = neh.get_cost();

    const int max_runtime = _jobs.size() * _factory.get_machine_size() / 2 * _t;

    _count = 0;
    unsigned non_improve_count = 0;
    unsigned t = _t0;

    int runtime = 0;
    int convergence_time = 0;

    while( runtime < max_runtime)
    {
        ConsDes consdes(_d, neh.get_result(), _factory, _sf);
        consdes.run();
        Jobs pi = consdes.get_result();
        unsigned cost = consdes.get_cost();
        if(_ls)
        {
            LSRandom ls(pi, _factory, _sf);
            ls.run();
            pi   = ls.get_result();
            cost = ls.get_cost();
        }
        bool accept = false;
        if(cost < pi_best_cost)
        {
            non_improve_count = 0;
            pi_best = pi;
            pi_best_cost = cost;
            accept = true;
        }
        else
        {
            ++non_improve_count;
            if(_is_accept(cost, pi_best_cost, t))
            {
                pi_best = pi;
                pi_best_cost = cost;
            }
        }
        ++_count;
        if(0 != _gamma && _count % _gamma == 0)
        {
            t = _alpha * t;
        }
        runtime += time.restart();
        if(accept)
        {
            convergence_time = runtime;
        }
    }
    _factory.add_jobs(pi_best);
    runtime += time.restart();

    printf("summary\n");
    printf("  count             : %u\n", _count);
    printf("  non-improve count : %u\n", non_improve_count);
    printf("  run time          : %d ms\n", runtime);
    printf("  convergence time  : %d ms\n", convergence_time);
    printf("  max run time      : %d ms\n", max_runtime);
    printf("\n");
}

bool IG::_is_accept(unsigned pi_purown, unsigned pi_new, unsigned t) const
{
    double ap    = exp( -( (double)( pi_purown - pi_new ) / t ) );
    double alpha = (double) rand() / (RAND_MAX + 1.0 );

    return alpha < ap;
}
