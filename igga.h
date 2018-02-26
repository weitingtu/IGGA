#ifndef IGGA_H
#define IGGA_H

#include "scheduler.h"

class IGGA : public Scheduler
{
public:
    IGGA(const Jobs& jobs, const Factory& factory, unsigned d, double jp);

    virtual void run();

private:
    bool _is_accept(unsigned pi_purown, unsigned pi_new) const;
    Jobs _crossvoer(const Jobs& pi_new, const Jobs& jobs);
    std::vector<Jobs> _crossvoer(const Jobs& pi_new, const std::vector<Jobs>& others);

    const unsigned _d;
    const double   _jp;
};

#endif // IGGA_H
