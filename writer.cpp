#include "writer.h"
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

Writer::Writer()
{
}

void Writer::_write(FILE* fp,
                    const std::string& file_name,
                    int seed,
                    int time,
                    unsigned iteration,
                    const Jobs& jobs,
                    const JobInfo& job_info
                    ) const
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

    fprintf(fp, "Filename, %s\n",  file_name.c_str());
    fprintf(fp, "Cmax, %u\n",       c_max);
    fprintf(fp, "Sum F, %u\n",      sum_f);
    fprintf(fp, "Seed, %d\n",       seed);
    fprintf(fp, "Time used, %d\n",  time);
    fprintf(fp, "Iterations, %u\n", iteration);
    fprintf(fp, "Run Time used, %d\n",     job_info.runtime);
    fprintf(fp, "Convergence Time, %d\n",  job_info.convergence_time);
    fprintf(fp, "Max run Time, %d\n",      job_info.max_runtime);
    fprintf(fp, "Count used, %d\n",        job_info.count);
    fprintf(fp, "Non-improve Count, %d\n", job_info.non_improve_count);
    fprintf(fp, "Convergence Count, %d\n", job_info.convergence_count);
    fprintf(fp, "Seq, Job-No, Complete Time (F)\n");
    for(size_t i = 0; i < jobs.size(); ++i)
    {
        const Job& job = jobs.at(i);
        fprintf(fp, "%zu, %u, %u\n", i, job.id, job.machine_times.back());
    }
    fprintf(fp, "\n");
}

void Writer::write(const std::string& in_name,
                   const std::vector<int>& seeds,
                   const std::vector<int>& times,
                   const std::vector<unsigned>& iterations,
                   const std::vector<Jobs>& job_sets,
                   const std::vector<JobInfo>& job_infos
                   ) const
{
    if(in_name.empty())
    {
        printf("error: empty file name\n");
        return;
    }
    if(job_sets.empty())
    {
        printf("error: empty job sets\n");
        return;
    }

    std::vector<std::string> tokens = split( in_name, '.' );
    std::string out_name = tokens.front() + ".csv";

    FILE* fp = fopen(out_name.c_str(), "w");

    if(nullptr == fp)
    {
        printf("error: failed to open %s\n", in_name.c_str());
        return;
    }

    for(size_t i = 0; i < job_sets.size(); ++i)
    {
        _write(fp, in_name, seeds.at(i), times.at(i), iterations.at(i), job_sets.at(i), job_infos.at(i));
    }

    fclose(fp);
}
