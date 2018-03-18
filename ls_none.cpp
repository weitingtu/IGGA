#include "ls_none.h"

LSNone::LSNone(const Jobs &jobs, const Factory &factory, const SeqFactory &sf):
    Scheduler(jobs, factory, sf)
{

}

void LSNone::run()
{
    _factory.add_jobs(_jobs);
    _result_jobs = _jobs;
}
