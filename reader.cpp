#include "reader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <QtGlobal>

Reader::Reader() :
    _jobs(),
    _factories()
{

}

bool Reader::_string2int( const std::string& str, int& result )
{
    try
    {
        std::size_t last_char;
        result = std::stoi( str, &last_char, 10 );
        return last_char == str.size();
    }
    catch ( std::invalid_argument& )
    {
        return false;
    }
    catch ( std::out_of_range& )
    {
        return false;
    }
}

bool Reader::read(char* path)
{
    std::ifstream ifs(path, std::ifstream::in);
    if(!ifs.good())
    {
        printf("failed to open %s\n", path);
        return;
    }

    std::string line;
    int num_jobs     = 0;
    int num_machines = 0;
    int count = 0;
    bool read_job = false;
    while(std::getline(ifs, line))
    {
        std::istringstream iss(line);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                              std::istream_iterator<std::string>{}};
        if(tokens.empty())
        {
            continue;
        }

        if(!read_job)
        {
            if(tokens.size() != 5)
            {
                continue;
            }
            if(!_string2int( tokens[0], num_jobs))
            {
                continue;
            }
            if(!_string2int( tokens[1], num_machines))
            {
                continue;
            }
            read_job = true;
            _jobs.push_back(std::vector<Job>(num_jobs));
            _factories.push_back(Factory(num_machines));
            continue;
        }

        int result;
        if(!_string2int( tokens[0], result))
        {
            continue;
        }

        if((int)tokens.size() != num_jobs)
        {
            printf("error: token size %zu and num of jobs %d mismatch\n", tokens.size(), num_jobs);
            return false;
        }

        for(size_t i = 0; i < tokens.size(); ++i)
        {
            int processing_time;
            if(!_string2int( tokens[i], processing_time))
            {
                printf("error: tokens[%zu] (%s) is not integer\n", i, tokens[i].c_str());
                return false;
            }
            _jobs.back().at(i).processing_times.push_back(processing_time);
        }

        ++count;
        if(num_machines == count)
        {
            num_jobs = 0;
            num_machines = 0;
            count = 0;
            read_job = false;
        }
    }

    ifs.close();
    return true;
}
