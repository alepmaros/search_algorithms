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

#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <SFML/Graphics.hpp>

#include <list>
#include <deque>
#include <queue>

#include "util.h"
#include "node.h"
#include "map.h"

//struct SortByPath
//{
    //bool operator()(const int &a, const int &b) const
    //{
        //return a->getPathCost() < b->getPathCost();
    //}
//};

class Pathfinder
{
    public:
        Pathfinder(Map &map);

        void    draw(sf::RenderWindow *window);
        void    update();

        void    setSearchAlgorithm(bs::SearchAlgorithm sa);
        void    calculatePath(bs::SearchAlgorithm sa);

        std::pair<int,int>  makePath();

    private:
        void    initializeSearch();
        void    printStatistics(std::string searchAlgo, int numCells,
                int cost);

        void    breadthSearch();

        void    uniformCostSearch();

        void    IDSearch();

        void    AStarSearch();

    private:
        Map                             &mMap;
        bs::SearchAlgorithm             mCurrentSearchAlgorithm;

        // Variables used in the algorithms
        // mVisited = stores if position was already visited
        // mOpen = list of nodes yet to be visited
        // mLevel = tree level for ITS
        std::vector<std::vector<bool> > mVisited;
        std::deque<Node*>               mOpen;

        // Control variables for IDS
        int                             mMaxLevel;
        bool                            mReachedLevel;

        int                             mNumNodesOpened;
        sf::Vector2i                    mStartPosition;
        sf::Vector2i                    mEndPosition;
        sf::Vector2i                    mGridSize;

        // For visualization of the algorithm
        sf::Clock                       mAnimationTimer;
        bool                            mFoundPath;
        bool                            mPrintedStats;
        std::vector<sf::CircleShape>    mPoints;
        std::vector<sf::RectangleShape> mLines;
        std::vector<sf::RectangleShape> mNodesVisited;
        int                             mCellsOnPath;

        // Control Variables

};

#endif
