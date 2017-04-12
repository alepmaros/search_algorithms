#ifndef UTIL_H
#define UTIL_H

namespace bs
{
    enum FloorType
    {
        Grass,
        Hill,
        Swamp,
        Fire
    };

    // BS = Breadth Search
    // DS = Depth Search
    // UCS = Uniform Cost Search
    enum SearchAlgorithm
    {
        None,
        BS,
        DS,
        UCS
    };
}

#endif
