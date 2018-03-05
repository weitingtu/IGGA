#include "scheduler.h"

Scheduler::Scheduler(const Jobs &jobs, const Factory &factory, const SeqFactory &sf)
    : _jobs(jobs),
      _factory(factory),
      _sf(sf),
      _result_jobs(),
      _count(0),
      _non_improve_count(0),
      _convergence_count(0),
      _runtime(0),
      _convergence_time(0),
      _max_runtime(0)
{

}
