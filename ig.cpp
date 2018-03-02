#include "ig.h"
#include "neh.h"
#include "consdes.h"
#include "possibility.h"
#include <stdlib.h>
#include <QTime>

IG::IG(const Jobs& jobs, const Factory& factory, unsigned d, unsigned t0, double alpha, unsigned gamma)
    : Scheduler(jobs, factory),
      _d(d),
      _t(30),
      _t0(t0),
      _alpha(alpha),
      _gamma(gamma)
{

}

void IG::run()
{
    NEH neh(_jobs, _factory);
    neh.run();
    Jobs pi_best          = neh.get_result();
    unsigned pi_best_cost = neh.get_cost();

    const unsigned max_run_time = _jobs.size() * _factory.get_machine_size() / 2 * _t;

    _count = 0;
    unsigned non_improve_count = 0;
    unsigned t = _t0;

    unsigned run_time = 0;
    unsigned convergence_time = 0;

    QTime time;
    time.start();

    while( run_time < max_run_time)
    {
        time.restart();
        ConsDes consdes(_d, neh.get_result(), _factory);
        consdes.run();
        consdes.get_result();
        bool accept = false;
        if(consdes.get_cost() < pi_best_cost)
        {
            non_improve_count = 0;
            pi_best = consdes.get_result();
            pi_best_cost = consdes.get_cost();
            accept = true;
        }
        else
        {
            ++non_improve_count;
            if(_is_accept(consdes.get_cost(), pi_best_cost, t))
            {
                pi_best = consdes.get_result();
                pi_best_cost = consdes.get_cost();
            }
        }
        ++_count;
        if(0 != _gamma && _count % _gamma == 0)
        {
            t = _alpha * t;
        }
        run_time += time.elapsed();
        if(accept)
        {
            convergence_time = run_time;
        }
    }
    printf("summary\n");
    printf("  count             : %u\n", _count);
    printf("  non-improve count : %u\n", non_improve_count);
    printf("  run time          : %u\n", run_time);
    printf("  convergence time  : %u\n", convergence_time);
    printf("  max run time      : %u\n", max_run_time);
    printf("\n");

    _factory.add_jobs(pi_best);
}

bool IG::_is_accept(unsigned pi_purown, unsigned pi_new, unsigned t) const
{
    double ap    = exp( -( (double)( pi_purown - pi_new ) / t ) );
    double alpha = (double) rand() / (RAND_MAX + 1.0 );

    return alpha < ap;
}
