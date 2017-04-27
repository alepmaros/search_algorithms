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
