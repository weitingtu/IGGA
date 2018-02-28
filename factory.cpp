#include "factory.h"
#include <stdlib.h>
#include <stdio.h>
#include <QtGlobal>

Factory::Factory(unsigned num_of_machine):
    _machine_times(num_of_machine, 0)
{
}

void Factory::print() const
{
    for(const Job& job : _jobs)
    {
        printf("Job %u ", job.id);
        for(auto i : job.machine_times)
        {
            printf("%u ", i);
        }
        printf("\n");
    }
    printf("cost %u\n", get_cost());
}

unsigned Factory::_add_job(const Job& new_job)
{
    if(new_job.processing_times.size() != _machine_times.size())
    {
        printf("Job processing times size %zu and machine times size %zu mismatch\n",
               new_job.processing_times.size(),
               _machine_times.size());
        Q_ASSERT(0);
        return 0;
    }

    _jobs.push_back(new_job);
    Job& job = _jobs.back();
    job.machine_times.clear();

    unsigned total_time = _machine_times.at(0);
    for(size_t i = 0; i < job.processing_times.size(); ++i)
    {
        total_time += job.processing_times[i];
        job.machine_times.push_back(total_time);
    }

    for(size_t i = 0; i < job.machine_times.size() - 1;++i)
    {
        if(job.machine_times[i] >= _machine_times.at(i + 1))
        {
            continue;
        }
        unsigned shift = _machine_times.at(i + 1) - job.machine_times[i];
        std::transform(job.machine_times.begin(), job.machine_times.end(),
                       job.machine_times.begin(), bind2nd(std::plus<unsigned>(), shift));

    }
    _machine_times = job.machine_times;
    return _machine_times.back();
}

unsigned Factory::add_jobs(const Jobs &jobs)
{
    _jobs.clear();
    std::fill( _machine_times.begin(), _machine_times.end(), 0);

    unsigned cost = 0;
    for(size_t i = 0; i < jobs.size(); ++i)
    {
        cost += _add_job(jobs[i]);
    }
    return cost;
}

unsigned Factory::get_cost() const
{
    unsigned cost = 0;
    for(size_t i = 0; i < _jobs.size(); ++i)
    {
        cost += _jobs[i].machine_times.back();
    }
    return cost;
}

unsigned SeqFactory::_d(const Job& ji, const Job& jj) const
{
    int max = std::numeric_limits<int>::min();
    int jj_total = std::accumulate(jj.processing_times.begin(), jj.processing_times.end(), 0);
    size_t m = ji.processing_times.size();
    for(size_t i = 0; i < m; ++i)
    {
        int jj_sub = std::accumulate(jj.processing_times.begin(), jj.processing_times.begin() + i, 0);
        int ji_total = std::accumulate(ji.processing_times.begin() + i + 1, ji.processing_times.end(), 0);

        max = std::max(max, jj_total - jj_sub - ji_total);
    }
    return (unsigned) max;
}

void SeqFactory::init(const Jobs& jobs)
{
    _d_matrix.resize(jobs.size());
    for(size_t i = 0; i < _d_matrix.size(); ++i)
    {
        _d_matrix.at(i).resize(jobs.size(), 0);
    }

    for(size_t i = 0; i < jobs.size(); ++i)
    {
        for(size_t j = 0; j < jobs.size(); ++j)
        {
            if( i != j)
            {
                _d_matrix.at(i).at(j) = _d(jobs.at(i), jobs.at(j));
            }
        }
    }
}

void SeqFactory::print() const
{
    for(size_t i = 0; i < _d_matrix.size(); ++i)
    {
        for(size_t j = 0; j < _d_matrix.at(i).size(); ++j)
        {
            printf("%u ", _d_matrix.at(i).at(j));
        }
        printf("\n");
    }
}

unsigned SeqFactory::tct(const Jobs& jobs) const
{
    if(jobs.empty())
    {
        return 0;
    }

    unsigned cost = 0;
    const Job& j0 = jobs.front();
    cost += jobs.size() * std::accumulate(j0.processing_times.begin(), j0.processing_times.end(), 0 );

    for(size_t j = 1; j < jobs.size(); ++j)
    {
        const Job& jj_1 = jobs.at(j - 1);
        const Job& jj   = jobs.at(j);
        cost += (jobs.size() - j) * _d_matrix.at(jj_1.id).at(jj.id);
    }

    return cost;
}

unsigned SeqFactory::seq_tct(const JobsSeq& jobs) const
{
    if(jobs.empty())
    {
        return 0;
    }

    unsigned cost = 0;
    auto ite = jobs.begin();
    const Job& j0 = *ite;
    cost += jobs.size() * std::accumulate(j0.processing_times.begin(), j0.processing_times.end(), 0 );

    for(size_t j = 1; j < jobs.size(); ++j)
    {
        const Job& jj_1 = *(ite);
        const Job& jj   = *(++ite);
        cost += (jobs.size() - j) * _d_matrix.at(jj_1.id).at(jj.id);
    }

    return cost;
}
