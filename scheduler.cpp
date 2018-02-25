#include "scheduler.h"

Scheduler::Scheduler(const Jobs &jobs, const Factory &factory)
    : _jobs(jobs), _factory(factory)
{

}
