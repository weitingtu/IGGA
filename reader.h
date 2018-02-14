#ifndef READER_H
#define READER_H

#include "job.h"
#include "factory.h"
#include <string>

class Reader
{
public:
    Reader();
    bool read(char* path);

private:
    bool _string2int( const std::string& str, int& result );

    std::vector<std::vector<Job> > _jobs;
    std::vector<Factory>           _factories;
};

#endif // READER_H
