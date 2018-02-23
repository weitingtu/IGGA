#include "writer.h"

Writer::Writer()
{
}

void Writer::_write(FILE* fp, const Jobs& jobs) const
{
    if(jobs.empty())
    {
        printf("error: empty jobs\n");
        return;
    }
    if(jobs.front().machine_times.empty())
    {
        printf("error: empty factory\n");
        return;
    }

    // Cmax
    // Sum F
    // Seq Job-No Complete Time (F)

    unsigned c_max = 0;
    unsigned sum_f = 0;

    c_max = jobs.back().machine_times.back();

    for(size_t i = 0; i < jobs.size(); ++i)
    {
        sum_f += jobs.at(i).machine_times.back();
    }

    fprintf(fp, "Cmax,  %u\n", c_max);
    fprintf(fp, "Sum F, %u\n", sum_f);
    fprintf(fp, "Seq, Job-No, Complete Time (F)\n");
    for(size_t i = 0; i < jobs.size(); ++i)
    {
        const Job& job = jobs.at(i);
        fprintf(fp, "%zu, %u, %u\n", i, job.id, job.machine_times.back());
    }
    fprintf(fp, "\n");
}

void Writer::write(const std::string& file_name, const std::vector<Jobs> &job_sets) const
{
    if(job_sets.empty())
    {
        printf("error: empty job sets\n");
        return;
    }

    FILE* fp = fopen(file_name.c_str(), "w");

    if(nullptr == fp)
    {
        printf("error: failed to open %s\n", file_name.c_str());
        return;
    }

    for(size_t i = 0; i < job_sets.size(); ++i)
    {
        _write(fp, job_sets.at(i));
    }

    fclose(fp);
}
