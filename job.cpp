#include "job.h"

Job::Job():
    id(0),
    processing_times(),
    machine_times(),
    _sum(0)
{

}

unsigned Job::get_sum() const
{
    if(0 == _sum)
    {
        _sum = std::accumulate(processing_times.begin(),processing_times.end(),0);
    }
    return _sum;
}
