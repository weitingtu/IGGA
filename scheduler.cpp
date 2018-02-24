#include "scheduler.h"

Scheduler::Scheduler(const Jobs &jobs, const Factory &Factory)
    : _jobs(jobs), _factory(Factory)
{

}
