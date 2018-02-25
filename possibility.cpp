#include "possibility.h"
#include <stdlib.h>

void Possibility::init()
{
    srand(0);
}

std::vector<int> Possibility::KnuthShuffle( int n, int low, int up)
{
    int size = up - low + 1;

    std::vector<int> poker(size);
    for(int i = 0 ; i < size; ++i)
    {
        poker[i] = i + low;
    }

    for(int i = size - 1; i > 0; --i)
    {
        int pos = ( int ) ( rand() / ( RAND_MAX + 1.0 ) * ( i + 1));
        std::swap(poker[pos], poker[i]);
    }

    return std::vector<int>(poker.begin(), poker.begin() + n);
}
