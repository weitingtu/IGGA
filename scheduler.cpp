#include "scheduler.h"

Scheduler::Scheduler(const Jobs &jobs, const Factory &factory, const SeqFactory &sf)
    : _jobs(jobs), _factory(factory), _sf(sf), _result_jobs(), _count(0)
{

}
