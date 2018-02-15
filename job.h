#ifndef JOB_H
#define JOB_H

#include <vector>
#include <algorithm>
#include <tuple>

class Job
{
public:
    Job();

    unsigned get_sum() const;

    friend bool operator< (const Job& lhs, const Job& rhs)
    {
        unsigned lhs_sum = lhs.get_sum();
        unsigned rhs_sum = rhs.get_sum();
        return std::tie(lhs_sum, lhs.id) < std::tie(rhs_sum, rhs.id);
    }
    friend bool operator> (const Job& lhs, const Job& rhs)
    {
        return rhs < lhs;
    }

    unsigned id;
    std::vector<unsigned> processing_times;
    std::vector<unsigned> machine_times;
private:
    mutable unsigned _sum;
};

#endif // JOB_H
