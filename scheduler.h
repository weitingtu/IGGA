#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "job.h"
#include "factory.h"

class Scheduler
{
public:
    Scheduler(const Jobs& jobs, const Factory& factory, const SeqFactory& sf);
    virtual ~Scheduler() {}

    virtual void run() = 0;

    const Jobs& get_result() const { return _factory.get_jobs(); }
    unsigned get_cost() const { return _factory.get_cost(); }
    const Jobs& get_result_jobs() const { return _result_jobs; }
    unsigned get_result_cost() const { return _sf.tct(_result_jobs); }
    unsigned get_count() const { return _count; }
    unsigned get_non_improve_count() const { return _non_improve_count; }
    unsigned get_convergence_count() const { return _convergence_count; }
    int get_runtime() const { return _runtime; }
    int get_convergence_time() const { return _convergence_time; }
    int get_max_runtime() const { return _max_runtime; }

protected:
    Jobs       _jobs;
    Factory    _factory;
    SeqFactory _sf;
    Jobs       _result_jobs;
    unsigned   _count;
    unsigned   _non_improve_count;
    unsigned   _convergence_count;
    int        _runtime;
    int        _convergence_time;
    int        _max_runtime;
};

#endif // SCHEDULER_H
