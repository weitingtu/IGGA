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

    bool empty() const { return _job_sets.empty(); }
    size_t size() const { return _job_sets.size(); }

    const Jobs&    get_jobs(size_t i) const  { return _job_sets.at(i); }
    const Factory& get_factory(size_t i) const { return _factories.at(i); }

private:
    bool _string2int( const std::string& str, int& result );
    void _clear() { _job_sets.clear(); _factories.clear(); }

    std::vector<Jobs>    _job_sets;
    std::vector<Factory> _factories;
};

#endif // READER_H
