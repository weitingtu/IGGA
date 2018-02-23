#include "cfi.h"
#include "neh.h"
#include <algorithm>
#include <QtGlobal>
#include "ioApi.h"

CFI::CFI(const Jobs &jobs, const Factory &factory):
    _jobs(jobs),
    _factory(factory)
{
}

Job CFI::_create_job(const Job& job, unsigned job_factor) const
{
    Job j = job;
    std::transform(j.processing_times.begin(), j.processing_times.end(),
                   j.processing_times.begin(), std::bind1st(std::multiplies<unsigned>(), job_factor));
    return j;
}

Job CFI::_create_artificial_job( const std::set<unsigned>& job_set) const
{
    Job j(_factory.get_machine_size());
    j.id = _jobs.size();
    for(size_t i = 0; i < _jobs.size(); ++i)
    {
        if(job_set.find(i) != job_set.end())
        {
            continue;
        }
        const Job& job = _jobs.at(i);
        for(size_t k = 0; k < job.processing_times.size(); ++k)
        {
            j.processing_times.at(k) += job.processing_times.at(k);
        }
    }

    return j;
}

unsigned CFI::_get_idle_time(const Job& j) const
{
    unsigned diff = 0;
    for(size_t i = 0; i < j.processing_times.size() - 1; ++i)
    {
        diff += j.processing_times.at(i);
    }
    return diff;
}

unsigned CFI::_get_idle_time(const Job& j1, const Job& j2) const
{
    Q_ASSERT(j1.machine_times.size() == j2.machine_times.size());

    unsigned diff = 0;
    for(size_t i = 0; i < j1.machine_times.size(); ++i)
    {
        diff += j2.machine_times.at(i) - j2.processing_times.at(i) - j1.machine_times.at(i);
//        printf("%u - %u - %u = %u\n",
//                    j2.machine_times.at(i),
//                    j2.processing_times.at(i),
//                    j1.machine_times.at(i),
//                    j2.machine_times.at(i) - j2.processing_times.at(i) - j1.machine_times.at(i));
    }

    return diff;
}

unsigned CFI::_get_index_function_value(const Jobs& pi, unsigned job_id)
{
    std::set<unsigned> job_set;
    unsigned job_factor = _jobs.size() - pi.size() - 1;
    Jobs s;
    for(size_t i = 0; i < pi.size(); ++i)
    {
        const Job& job = pi.at(i);
        s.push_back(_create_job(job, job_factor));
        job_set.insert(job.id);
    }

    s.push_back(_create_job(_jobs.at(job_id), job_factor));
    job_set.insert(job_id);

    s.push_back(_create_artificial_job(job_set));

    _factory.add_jobs(s);
    _factory.print();

    unsigned ci = pi.empty() ? _get_idle_time( _factory.get_jobs().at(pi.size()) )
                             : _get_idle_time( _factory.get_jobs().at(pi.size() - 1), _factory.get_jobs().at(pi.size()));
    unsigned fi = _get_idle_time( _factory.get_jobs().at(pi.size()), _factory.get_jobs().at(pi.size() + 1));

    return job_factor * ci + fi;
}

Jobs CFI::_isa()
{
    Jobs pi;
    std::set<unsigned> job_set;
    while(pi.size() < _jobs.size() - 1)
    {
        io_debug("Iteration %d\n", (int)pi.size());
        unsigned min_job_idx              = std::numeric_limits<unsigned>::max();
        unsigned min_index_function_value = std::numeric_limits<unsigned>::max();
        for(size_t i = 0; i < _jobs.size(); ++i)
        {
            if(job_set.find(i) != job_set.end())
            {
                continue;
            }
            unsigned index_function_value = _get_index_function_value(pi, i);
            io_debug("f(%u) = %u %f\n", i, index_function_value, (double)index_function_value / (_jobs.size() - pi.size() - 1));
            if(index_function_value < min_index_function_value)
            {
                min_index_function_value = index_function_value;
                min_job_idx = i;
            }
        }
        io_debug("min f(%u) = %u %f\n", min_job_idx, min_index_function_value, (double) min_index_function_value / (_jobs.size() - pi.size() - 1));
        pi.push_back(_jobs.at(min_job_idx));
        job_set.insert(min_job_idx);
    }

    Q_ASSERT(pi.size() == _jobs.size() - 1);

    for(size_t i = 0; i < _jobs.size(); ++i)
    {
        if(job_set.find(i) != job_set.end())
        {
            continue;
        }
        pi.push_back(_jobs.at(i));
    }

    _factory.add_jobs(pi);
    _factory.print();
    return pi;
}

Jobs CFI::_neh(Jobs pi)
{
    _factory.add_jobs(pi);
    _factory.print();

    Jobs best = pi;
    unsigned min_cost = _factory.get_cost();

    NEH neh(pi, _factory);

    unsigned r = 1;
    while(r <= 6)
    {
       ++r;
       pi = neh.run( pi );
       _factory.add_jobs( pi );
       unsigned cost = _factory.get_cost();
       if(cost < min_cost)
       {
           min_cost = cost;
           best = pi;
       }
    }

    return best;
}

void CFI::run()
{
    Jobs pi = _isa();
    pi = _neh(pi);
    _factory.add_jobs(pi);
    _factory.print();
    _jobs = _factory.get_jobs();
}
