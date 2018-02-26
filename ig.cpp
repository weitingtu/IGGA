#include "ig.h"
#include "neh.h"
#include "consdes.h"
#include "possibility.h"
#include <stdlib.h>

IG::IG(unsigned d, const Jobs &jobs, const Factory &factory)
    : Scheduler(jobs, factory),
      _d(d),
      _t0(100),
      _alpha(0.9),
      _gamma(20)
{

}

void IG::run()
{
    NEH neh(_jobs, _factory);
    neh.run();
    Jobs pi_best          = neh.get_result();
    unsigned pi_best_cost = neh.get_cost();

    const unsigned max_count = _jobs.size() * 200;
    const unsigned max_non_improve_count = 100;

    unsigned count = 0;
    unsigned non_improve_count = 0;
    unsigned t = _t0;
    while(count < max_count && non_improve_count < max_non_improve_count)
    {
        ConsDes consdes(_d, neh.get_result(), _factory);
        consdes.run();
        consdes.get_result();
        if(consdes.get_cost() < pi_best_cost)
        {
            non_improve_count = 0;
            pi_best = consdes.get_result();
            pi_best_cost = consdes.get_cost();
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
        ++count;
        if(count % _gamma == 0)
        {
            t = _alpha * t;
        }
    }

    _factory.add_jobs(pi_best);
}


bool IG::_is_accept(unsigned pi_purown, unsigned pi_new, unsigned t) const
{
    double ap    = exp( -( ( pi_purown - pi_new ) / t ) );
    double alpha = (double) rand() / (RAND_MAX + 1.0 );

    return alpha < ap;
}
