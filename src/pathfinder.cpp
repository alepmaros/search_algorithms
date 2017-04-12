#include "pathfinder.h"

#include <iostream>

Pathfinder::Pathfinder(Map &map)
    : mMap(map)
{
    mGridSize = map.getGridSize();

    for (int i = 0; i < mGridSize.y; i++)
    {
        mVisited.push_back(std::vector<bool>());
        for (int j = 0; j < mGridSize.x; j++)
        {
            mVisited[i].push_back(false);
        }
    }

    mCurrentSearchAlgorithm = bs::SearchAlgorithm::None;
}

void Pathfinder::update()
{

}

void Pathfinder::draw(sf::RenderWindow *window)
{

}

void Pathfinder::setSearchAlgorithm(bs::SearchAlgorithm sa)
{
    mCurrentSearchAlgorithm = sa;
}

void Pathfinder::calculatePath()
{
    if (mCurrentSearchAlgorithm == bs::SearchAlgorithm::None)
    {
        std::cout << "No search algorithm selected" << std::endl;
        return;
    }

    initializeSearch();

    if (mCurrentSearchAlgorithm == bs::SearchAlgorithm::BS)
    {
        breadthSearch();
    }
    else if (mCurrentSearchAlgorithm == bs::SearchAlgorithm::DS)
    {
        //depthSearch();
    }
    else if (mCurrentSearchAlgorithm == bs::SearchAlgorithm::UCS)
    {
        //uniformCostSearch();
    }

}

void Pathfinder::breadthSearch()
{
    while(! mVisited.empty() )
    {
        Node e = mOpen.front();
        mOpen.pop_front();
        mNodes.push_back(std::move(e));
        
        sf::Vector2i gridPos = e.getGridPos();

        // North
        if (gridPos.y - 1 > 0)
        {
            if (addNode(sf::Vector2i(gridPos.x, gridPos.y-1), &mNodes.back()))
            {
               return; 
            }
        }

        // East
        if (gridPos.x + 1 < mGridSize.x)
        {
            if (addNode(sf::Vector2i(gridPos.x+1, gridPos.y), &mNodes.back()))
            {
               return; 
            }
        }

        // South
        if (gridPos.y + 1 < mGridSize.y)
        {
            if (addNode(sf::Vector2i(gridPos.x, gridPos.y+1), &mNodes.back()))
            {
               return; 
            }
        }

        // West
        if (gridPos.x - 1 > 0)
        {
            if (addNode(sf::Vector2i(gridPos.x-1, gridPos.y), &mNodes.back()))
            {
               return; 
            }
        }
    }
}

// addNode will return true if the node that it is adding is the destination
bool Pathfinder::addNode(sf::Vector2i gridPos, Node *parent)
{
    if ( !mVisited[gridPos.x][gridPos.y-1] )
    {
        Node j(sf::Vector2i(gridPos.x, gridPos.y-1));
        j.setParent(&mNodes.back());

        mOpen.push_back(j);
        if (j.getGridPos() == mEndPosition)
            return true;
    }
    return false;
}

void Pathfinder::initializeSearch()
{
    mStartPosition = mMap.getStartPosition();
    mEndPosition = mMap.getEndPosition();

    // Add startPosition to node to be visited list
    mOpen.clear();
    mOpen.push_back(Node(mStartPosition));

    // Clear visited grid
    mGridSize = mMap.getGridSize();
    for (int i = 0; i < mGridSize.y; i++)
    {
        for (int j = 0; j < mGridSize.x; j++)
        {
            mVisited[i][j] = false;
        }
    }
}
