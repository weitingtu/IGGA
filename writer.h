#ifndef WRITER_H
#define WRITER_H

#include "job.h"

class Writer
{
public:
    Writer();

    void write(const std::string&file_name, const std::vector<Jobs> &job_sets) const;

private:
    void _write(FILE* fp, const Jobs& jobs) const;
};

#endif // WRITER_H
