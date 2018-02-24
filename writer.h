#ifndef WRITER_H
#define WRITER_H

#include "job.h"

class Writer
{
public:
    Writer();

    void write(const std::string&in_name, const std::vector<int> &times, const std::vector<unsigned> &iterations, const std::vector<Jobs> &job_sets) const;

private:
    void _write(FILE* fp, const std::__cxx11::string &file_name, int time, unsigned iteration, const Jobs& jobs) const;
};

#endif // WRITER_H
