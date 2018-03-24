#ifndef WRITER_H
#define WRITER_H

#include "job.h"

struct JobInfo
{
    unsigned count;
    unsigned non_improve_count;
    unsigned convergence_count;
    int      runtime;
    int      convergence_time;
    int      max_runtime;
};

class Writer
{
public:
    Writer();

    void write(const std::string&in_name, const std::vector<int> &seeds, const std::vector<int> &times,
               const std::vector<unsigned> &iterations,
               const std::vector<Jobs> &job_sets, const std::vector<JobInfo>& job_infos) const;

private:
    void _write(FILE* fp, const std::__cxx11::string &file_name, int seed, int time, unsigned iteration, const Jobs& jobs, const JobInfo &job_info) const;
};

#endif // WRITER_H
