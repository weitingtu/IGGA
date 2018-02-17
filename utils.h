#ifndef UTILS_H
#define UTILS_H

struct greater
{
    template<class T>
    bool operator()(T const &a, T const &b) const { return a > b; }
};

#endif // UTILS_H
