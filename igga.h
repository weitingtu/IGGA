#ifndef IGGA_H
#define IGGA_H

#include "scheduler.h"

class IGGA : public Scheduler
{
public:
    IGGA(const Jobs& jobs, const Factory& factory, const SeqFactory& sf,unsigned d, double jp, unsigned t);
    IGGA(const Jobs& jobs, const Factory& factory, const SeqFactory& sf,unsigned d, double jp, unsigned t,
         unsigned t0, unsigned alpha, unsigned gamma);

    virtual void run();

private:
    bool _is_accept(unsigned pi_purown, unsigned pi_new) const;
    bool _is_sa_accept(unsigned pi_purown, unsigned pi_new, unsigned t) const;
    Jobs _crossvoer(const Jobs& pi_new, const Jobs& jobs);
    std::vector<Jobs> _crossvoer(const Jobs& pi_new, const std::vector<Jobs>& others);

    const unsigned _d;
    const double   _jp;
    const unsigned _t;
    const unsigned _t0;
    const double   _alpha;
    const unsigned _gamma;
};

#endif // IGGA_H
