#ifndef IG_H
#define IG_H

#include "scheduler.h"

class IG : public Scheduler
{
public:
    IG(const Jobs& jobs, const Factory& factory, unsigned d, unsigned t0, double alpha, unsigned gamma);

    virtual void run();
private:
    bool _is_accept(unsigned pi_purown, unsigned pi_new, unsigned t) const;

    const unsigned _d;
    const unsigned _t0;
    const double   _alpha;
    const unsigned _gamma;
};

#endif // IG_H
