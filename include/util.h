/*
 * Alexandre Maros - 2017/1
 * Search Algorithms
 *
 * This algorithms implements search algorithms such as BFS, Uniform Cost Search,
 * Iterative Deepening Search and A* search (With different weights for the
 * heuristic function).
 *
 * The this implementation is not fully optimized. Algorithms that need to be
 * sorted by pathCost such as UCS an A* should've been using a priority queue instead
 * of a list that is sorted after every modificaiton, but I did it like that to save me
 * some time. This was a work for my course and I did not have a lot of time.
 *
 * This isn't as comented as my normal code but it should be understandable.
 *
 * A lot of stuff was taken from the book: Artificial Intelligence: A Modern Approach
 * by Peter Norvig.
 *
 */

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
    // IDS = Iterative Deepening Search
    // AStar = A*
    enum SearchAlgorithm
    {
        None,
        BFS,
        DFS,
        UCS,
        AStar,
        IDS
    };
}

#endif
