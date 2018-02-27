#include "fnm.h"
#include "neh.h"
#include "utils.h"

struct JobSeq
{
    JobSeq() : id(0), sum(0), count(0)
    {}

    friend bool operator< (const JobSeq& lhs, const JobSeq& rhs)
    {
        return std::tie(lhs.count, rhs.sum, lhs.id) < std::tie(rhs.count, lhs.sum, rhs.id);
    }
    friend bool operator> (const JobSeq& lhs, const JobSeq& rhs)
    {
        return rhs < lhs;
    }

    unsigned id;
    unsigned sum;
    unsigned count;
};

FNM::FNM(const Jobs &jobs, const Factory &factory)
    : Scheduler(jobs, factory)
{
}

void FNM::run()
{
    Jobs pi = _init();
    pi = _neh(pi);
    _factory.add_jobs(pi);
//    _factory.print();
}

Jobs FNM::_init()
{
    std::vector<JobSeq> job_seqs(_jobs.size());
    for(size_t i = 0; i < job_seqs.size(); ++i)
    {
        job_seqs.at(i).id  = i;
        job_seqs.at(i).sum = _jobs.at(i).get_sum();
    }

    for(size_t i = 0; i < _jobs.size() - 1; ++i)
    {
        for(size_t j = i + 1; j < _jobs.size(); ++j)
        {
            _factory.add_jobs({_jobs.at(i), _jobs.at(j)});
            unsigned cost_1 = _factory.get_cost();
            _factory.add_jobs({_jobs.at(j), _jobs.at(i)});
            unsigned cost_2 = _factory.get_cost();

            if(cost_1 < cost_2)
            {
                job_seqs.at(i).count++;
            }
            else if(cost_1 > cost_2)
            {
                job_seqs.at(j).count++;
            }
        }
    }

    std::sort(job_seqs.begin(), job_seqs.end(), greater());

    Jobs pi;
    for(size_t i = 0; i < job_seqs.size(); ++i)
    {
        pi.push_back(_jobs.at(job_seqs.at(i).id));
    }
//    _factory.add_jobs(pi);
//    _factory.print();

    return pi;
}

Jobs FNM::_neh(Jobs pi)
{
    NEH neh(pi, _factory);

    pi = neh.run( pi );

    return pi;
}
