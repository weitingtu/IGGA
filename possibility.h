#ifndef POSSIBILITY_H
#define POSSIBILITY_H

#include <vector>

class Possibility
{
public:
    static void init();
    static std::vector<int> KnuthShuffle( int n, int low, int up);
};

#endif // POSSIBILITY_H
