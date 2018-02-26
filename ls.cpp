#include "ls.h"
#include <QtGlobal>
#include <set>

struct MachineJob
{
public:
    friend bool operator< (const MachineJob& lhs, const MachineJob& rhs)
    {
        return std::tie(lhs.processing_time, lhs.id) < std::tie(rhs.processing_time, rhs.id);
    }
    unsigned id;
    unsigned processing_time;
};

using MachineJobs = std::vector<MachineJob>;

class Machine
{
    public:
    Machine(size_t m) : jobs(m)
    {
        for(size_t i = 0; i < jobs.size(); ++i)
        {
            MachineJob& job = jobs.at(i);
            job.id = i;
            job.processing_time = 0;
        }
    }
    unsigned get_sum() const
    {
        unsigned sum = 0;
        for(const MachineJob& job : jobs)
        {
            sum += job.processing_time;
        }
        return sum;
    }
    Machine& operator+=(const Machine& rhs)
    {
        Q_ASSERT(jobs.size() == rhs.jobs.size());
        for(size_t i = 0; i < jobs.size(); ++i)
        {
            Q_ASSERT(jobs.at(i).id == i);
            Q_ASSERT(rhs.jobs.at(i).id == i);
            jobs.at(i).processing_time += rhs.jobs.at(i).processing_time;
        }
        return *this;
    }

    friend Machine operator+(Machine lhs, const Machine& rhs)
    {
      lhs += rhs;
      return lhs;
    }
    MachineJobs jobs;
};

using Machines = std::vector<Machine>;

LS::LS(const Jobs &jobs, const Factory& factory)
    : Scheduler(jobs, factory)
{

}

void LS::run()
{
    if(_factory.get_machine_size() < 1)
    {
        printf("error: machine size %zu too small\n", _factory.get_machine_size());
        return;
    }
    if(_jobs.empty())
    {
        printf("error: empty jobs\n");
        return;
    }

    Machines machines(_factory.get_machine_size(), Machine(_jobs.size()));

    for(size_t i = 0; i < _jobs.size(); ++i)
    {
        const Job& job = _jobs.at(i);
        for(size_t j = 0; j < job.processing_times.size(); ++j)
        {
            Machine& machine            = machines.at(j);
            MachineJob& machine_job     = machine.jobs.at(i);
            Q_ASSERT(machine_job.id == i);
            Q_ASSERT(machine_job.id == job.id);
            machine_job.processing_time = job.processing_times.at(j);
        }
    }

    Jobs best;
    unsigned min_cost = std::numeric_limits<unsigned>::max();
    Machine best_machine(_jobs.size());
    std::set<unsigned> idx_set;
    while(idx_set.size() < machines.size())
    {
        unsigned max_idx = std::numeric_limits<unsigned>::max();
        unsigned max_sum = 0;
        Machine max_machine(_jobs.size());
        for(size_t i = 0; i < machines.size();++i)
        {
            if(idx_set.find(i) != idx_set.end())
            {
                continue;
            }
            Machine machine = best_machine;
            machine += machines.at(i);
            unsigned sum = machine.get_sum();
            if(sum > max_sum)
            {
                max_idx = i;
                max_sum = sum;
                max_machine = machine;
            }
        }
        Q_ASSERT(max_idx != std::numeric_limits<unsigned>::max());
        idx_set.insert(max_idx);
        best_machine = max_machine;

        MachineJobs machine_jobs = best_machine.jobs;
        std::sort(machine_jobs.begin(), machine_jobs.end());
        Jobs pi;
        for(size_t i = 0; i < machine_jobs.size();++i)
        {
            pi.push_back(_jobs.at(machine_jobs.at(i).id));
        }
        _factory.add_jobs(pi);
//        _factory.print();
        unsigned cost = _factory.get_cost();
        if(cost < min_cost)
        {
            best = pi;
            min_cost = cost;
        }
    }
    _factory.add_jobs(best);
    _factory.print();
}
