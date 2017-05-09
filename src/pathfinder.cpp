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

#include "pathfinder.h"

#include <iostream>
#include <algorithm>
#include <cmath>

Pathfinder::Pathfinder(Map &map)
    : mMap(map)
{
    mGridSize = map.getGridSize();
    mAnimationTimer.restart();
    mPrintedStats = false;
    mNumNodesOpened = 0;

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
    if (mPrintedStats || mCurrentSearchAlgorithm == bs::SearchAlgorithm::None) 
    {
        return;
    }

    if (mCurrentSearchAlgorithm == bs::SearchAlgorithm::BFS)
    {
        breadthSearch();
    }
    else if (mCurrentSearchAlgorithm == bs::SearchAlgorithm::IDS)
    {
        if (mReachedLevel && mOpen.empty() 
                && mAnimationTimer.getElapsedTime().asMilliseconds() > 200)
        {
            int oldNumNodesOpened = mNumNodesOpened;
            initializeSearch();
            mNumNodesOpened += oldNumNodesOpened;
            mMaxLevel++;
        }
        IDSearch();
    }
    else if (mCurrentSearchAlgorithm == bs::SearchAlgorithm::UCS)
    {
        uniformCostSearch();
    }
    else if (mCurrentSearchAlgorithm == bs::SearchAlgorithm::AStar)
    {
        AStarSearch();
    }
}

void Pathfinder::draw(sf::RenderWindow *window)
{
    for (std::vector<sf::RectangleShape>::iterator it = mNodesVisited.begin();
            it != mNodesVisited.end(); it++)
    {
        window->draw(*it);
    }

    for (std::vector<sf::RectangleShape>::iterator it = mLines.begin(); 
            it != mLines.end(); it++)
    {

        window->draw(*it);
    }

    for (std::vector<sf::CircleShape>::iterator it = mPoints.begin();
            it != mPoints.end(); it++)
    {
        window->draw(*it);
    }
}

void Pathfinder::setSearchAlgorithm(bs::SearchAlgorithm sa)
{
    mCurrentSearchAlgorithm = sa;
}

void Pathfinder::calculatePath(bs::SearchAlgorithm sa)
{
    mCurrentSearchAlgorithm = sa;

    if (mCurrentSearchAlgorithm == bs::SearchAlgorithm::None)
    {
        std::cout << "No search algorithm selected" << std::endl;
        return;
    }

    initializeSearch();
    mMaxLevel = 0;
}

void Pathfinder::initializeSearch()
{
    mStartPosition = mMap.getStartPosition();
    mEndPosition = mMap.getEndPosition();
    mGridSize = mMap.getGridSize();

    mLines.clear();
    mPoints.clear();
    mNodesVisited.clear();
    mFoundPath = false;
    mPrintedStats = false;
    mReachedLevel = false;
    mNumNodesOpened = 0;
    
    //std::cout << mStartPosition.x << " " << mStartPosition.y << std::endl;
    //std::cout << mEndPosition.x << " " << mEndPosition.y << std::endl;

    // Add startPosition to node to be visited list
    mOpen.clear();
    mOpen.push_back(&mMap.mGrid[mStartPosition.y][mStartPosition.x]);

    // Clear visited grid
    for (int i = 0; i < mGridSize.y; i++)
    {
        for (int j = 0; j < mGridSize.x; j++)
        {
            mVisited[i][j] = false;
            mMap.mGrid[i][j].setParent(nullptr);
        }
    }
    mOpen.back()->setVisited();
}

void Pathfinder::printStatistics(std::string searchAlgo, int numCells, int cost)
{
    std::cout << "----- " << searchAlgo << " Search Statistics -----" << std::endl;
    std::cout << "Start position: (" << mStartPosition.x << "," << 
        mStartPosition.y << ")" << std::endl;
    std::cout << "End position: (" << mEndPosition.x << "," << 
        mEndPosition.y << ")" << std::endl;
    std::cout << "Number of opened nodes: " << mNumNodesOpened << std::endl;
    std::cout << "Number of cells on path: " << numCells << std::endl;
    std::cout << "Cost: " << cost << std::endl;
    std::cout << "----- " << searchAlgo << " Search Statistics -----" << std::endl << std::endl;
}

void Pathfinder::breadthSearch()
{
    if (mAnimationTimer.getElapsedTime().asMilliseconds() < 1)
        return;

    while( !mOpen.empty() && !mFoundPath )
    {
        Node *pe = mOpen.front();
        mOpen.pop_front();

        std::vector<Node*> adjacentNodes = pe->getAdjacentNodes();
        for(std::vector<Node*>::iterator it = adjacentNodes.begin();
                it != adjacentNodes.end(); it++)
        {
            if ( !(*it)->wasVisited() )
            {
                mNumNodesOpened++;
                mOpen.push_back((*it));
                mOpen.back()->setParent(pe);
                
                // To visualize the visited node
                float blockSize = mMap.getBlockSize();

                sf::RectangleShape n;
                n.setSize(sf::Vector2f(blockSize, blockSize));
                n.setPosition( (*it)->getPosition() );
                n.setFillColor(sf::Color(0,0,0,170));
                mNodesVisited.push_back(n);

                if (mOpen.back()->getGridPos() == mEndPosition)
                {
                    mFoundPath = true;
                    break;
                }
            }
        }

        // This is used so I can draw the nodes while updating. It does not 
        // belong to the search algorithm but it is a silly hack so I can draw
        // the nodes visited and make a cool animation of how the algo
        // works
        if ( !mOpen.empty() ) {
            mAnimationTimer.restart();
            return;
        }
    }

    
    // Move last element
    if ( !mOpen.empty() )
    {
        mPrintedStats = true;
        mNodesVisited.clear();
        std::pair<int, int> stats = makePath();
        printStatistics("Breadth", stats.first, stats.second);
    }
    
}

void Pathfinder::uniformCostSearch()
{
    if (mAnimationTimer.getElapsedTime().asMilliseconds() < 1)
        return;

    while( !mOpen.empty() && !mFoundPath )
    {
        Node *pe = mOpen.front();
        mOpen.pop_front();

        sf::Vector2i gridPos = pe->getGridPos();
        mVisited[gridPos.y][gridPos.x] = true;
        mNumNodesOpened++;

        if (pe->getGridPos() == mEndPosition)
        {
            mFoundPath = true;
            break;
        }

        std::vector<Node*> adjacentNodes = pe->getAdjacentNodes();
        for(std::vector<Node*>::iterator it = adjacentNodes.begin();
                it != adjacentNodes.end(); it++)
        {
            Node *p = (*it);
            if ( !mVisited[p->getGridPos().y][p->getGridPos().x] )
            {

                sf::Vector2i pGridPos = p->getGridPos();

                std::deque<Node*>::iterator it = mOpen.end();
                it = std::find_if(mOpen.begin(), mOpen.end(), 
                        [&pGridPos](const Node *a) -> bool
                        {
                            return a->getGridPos() == pGridPos;
                        });

                if (it != mOpen.end())
                {
                    if ((*it)->getPathCost() > (pe->getPathCost() + p->getCost()))
                        (*it)->setParent(pe);
                }
                else
                {
                    p->setParent(pe);
                    mOpen.push_back(p);
                }

                // To visualize the visited node
                float blockSize = mMap.getBlockSize();

                sf::RectangleShape n;
                n.setSize(sf::Vector2f(blockSize, blockSize));
                n.setPosition(mMap.mGrid[gridPos.y][gridPos.x].getPosition()
                        + sf::Vector2f(blockSize * mGridSize.y + 42 * mMap.getBlockGap(), 0));
                n.setFillColor(sf::Color(255,0,0,50));
                mNodesVisited.push_back(n);

            }
        }

        std::sort(mOpen.begin(), mOpen.end(),
                [](const Node *a, const Node *b) -> bool
                {
                    return a->getPathCost() < b->getPathCost();
                });

        // This is used so I can draw the nodes while updating. It does not 
        // belong to the search algorithm but it is a silly hack so I can draw
        // the nodes visited and make a cool animation of how the algo
        // works
        if ( !mOpen.empty() ) {
            mAnimationTimer.restart();
            return;
        }
    }

    
    // Move last element
    if ( !mOpen.empty() )
    {
        //mNodesVisited.clear();
        mPrintedStats = true;
        std::pair<int, int> stats = makePath();
        printStatistics("Uniform Cost", stats.first, stats.second);
    }
    
}

void Pathfinder::IDSearch()
{
    //if (mAnimationTimer.getElapsedTime().asMilliseconds() < 0)
        //return;

    while( !mOpen.empty() && !mFoundPath )
    {
        Node *pe = mOpen.front();
        mOpen.pop_front();

        if (pe->getLevel() > mMaxLevel)
        {
            mReachedLevel = true;
            mAnimationTimer.restart();
            continue;
        }

        int level = pe->getLevel();
        std::vector<Node*> adjacentNodes = pe->getAdjacentNodes();
        for(std::vector<Node*>::reverse_iterator it = adjacentNodes.rbegin();
                it != adjacentNodes.rend(); it++)
        {
            bool hasLowerLevel = level+1 < (*it)->getLevel();
            if ( !(*it)->wasVisited() || hasLowerLevel )
            {
                mNumNodesOpened++;
                mOpen.push_front((*it));
                mOpen.front()->setParent(pe);
                
                if (!hasLowerLevel)
                {
                    // To visualize the visited node
                    float blockSize = mMap.getBlockSize();
                    sf::RectangleShape n;
                    n.setSize(sf::Vector2f(blockSize, blockSize));
                    n.setPosition((*it)->getPosition());
                    n.setFillColor(sf::Color(0,0,0,170));
                    mNodesVisited.push_back(n);
                }

                if (mOpen.front()->getGridPos() == mEndPosition)
                {
                    mFoundPath = true;
                    break;
                }
            }

        }

        // This is used so I can draw the nodes while updating. It does not 
        // belong to the search algorithm but it is a silly hack so I can draw
        // the nodes visited and make a cool animation of how the algo
        // works
        if ( !mOpen.empty() ) {
            mAnimationTimer.restart();
            return;
        }
    }

    
    // Move last element
    if ( !mOpen.empty() )
    {
        mFoundPath = true;
        mPrintedStats = true;
        mNodesVisited.clear();
        std::pair<int,int> stats = makePath();
        printStatistics("Depth", stats.first, stats.second);
    }
    
}

void Pathfinder::AStarSearch()
{
    if (mAnimationTimer.getElapsedTime().asMilliseconds() < 1)
        return;

    while( !mOpen.empty() && !mFoundPath )
    {
        Node *pe = mOpen.front();
        mOpen.pop_front();
        mNumNodesOpened++;

        sf::Vector2i gridPos = pe->getGridPos();
        mVisited[gridPos.y][gridPos.x] = true;

        std::vector<Node*> adjacentNodes = pe->getAdjacentNodes();
        for(std::vector<Node*>::iterator it = adjacentNodes.begin();
                it != adjacentNodes.end(); it++)
        {
            Node *p = (*it);

            if ( !mVisited[p->getGridPos().y][p->getGridPos().x] )
            {

                sf::Vector2i pGridPos = p->getGridPos();

                // Manhattan Distance
                float distanceToGoal = std::abs( (pGridPos.x - mEndPosition.x)) +
                        std::abs(pGridPos.y - mEndPosition.y);

                float weight = 1.0;

                float possiblePathCost = (weight * distanceToGoal) + (p->getCost() + pe->getPathCost());

                std::deque<Node*>::iterator it = mOpen.end();
                it = std::find_if(mOpen.begin(), mOpen.end(), 
                        [&pGridPos](const Node *a) -> bool
                        {
                            return a->getGridPos() == pGridPos;
                        });

                if (it != mOpen.end())
                {
                    if (possiblePathCost < (*it)->getPossiblePathCost()) 
                    {
                        (*it)->setParent(pe);
                        (*it)->setPossiblePathCost(possiblePathCost);
                    }
                }
                else
                {
                    p->setParent(pe);
                    p->setPossiblePathCost(possiblePathCost);
                    mOpen.push_back(p);
                }

                // To visualize the visited node
                float blockSize = mMap.getBlockSize();

                sf::RectangleShape n;
                n.setSize(sf::Vector2f(blockSize, blockSize));
                n.setPosition(mMap.mGrid[gridPos.y][gridPos.x].getPosition()
                        + sf::Vector2f(blockSize * mGridSize.y + 42 * mMap.getBlockGap(), 0));
                n.setFillColor(sf::Color(255,0,0,50));
                mNodesVisited.push_back(n);

                if (pGridPos == mEndPosition)
                {
                    mFoundPath = true;
                    break;
                }
            }
        }

        std::sort(mOpen.begin(), mOpen.end(),
                [](const Node *a, const Node *b) -> bool
                {
                    return a->getPossiblePathCost() < b->getPossiblePathCost();
                });

        // This is used so I can draw the nodes while updating. It does not 
        // belong to the search algorithm but it is a silly hack so I can draw
        // the nodes visited and make a cool animation of how the algo
        // works
        if ( !mOpen.empty() ) {
            mAnimationTimer.restart();
            return;
        }
    }

    
    // Move last element
    if ( !mOpen.empty() )
    {
        //mNodesVisited.clear();
        mPrintedStats = true;
        std::pair<int, int> stats = makePath();
        printStatistics("A* Cost", stats.first, stats.second);
    }
    
}

std::pair<int,int> Pathfinder::makePath()
{
    int cells = 1;
    int cost = 0;

    float blockGap = mMap.getBlockGap();
    float blockSize = mMap.getBlockSize();
    float lineThickness = 5.0;
    float hexSize = lineThickness / 1.5;
    Node *n = &mMap.mGrid[mEndPosition.y][mEndPosition.x];
    Node *n_next;

    sf::Vector2f lineSize(blockSize + blockGap, lineThickness);

    while(n != nullptr)
    {
        cells++;
        cost += n->getCost();

        if (n->getParent() != nullptr)
        {
            n_next = n->getParent();

            sf::Vector2i nGridPos = n->getGridPos();
            sf::Vector2i nNextGridPos = n_next->getGridPos();

            //std::cout << "(" << n->getGridPos().x << ", " << n->getGridPos().y << ")" << std::endl;

            // Calculate the middle point of the block on pos nGridPos
            float middleX = n->getPosition().x + (blockSize/2.0);
            float middleY = n->getPosition().y + (blockSize/2.0);

            // LINE
            sf::RectangleShape line(lineSize);
            line.setFillColor(sf::Color::White);
            line.setOrigin(0, lineThickness / 2.0);

            if (nGridPos.x == nNextGridPos.x)
                line.rotate(90);

            sf::Vector2f linePos(middleX, middleY);

            if (nGridPos.y > nNextGridPos.y)
                linePos.y -= blockSize;

            if (nGridPos.x > nNextGridPos.x)
                linePos.x -= blockSize;

            line.setPosition(linePos);
            mLines.push_back(line);

            // HEXAGON
            sf::CircleShape hex(hexSize, 6);
            hex.setFillColor(sf::Color(0,0,0,0));
            hex.setOutlineColor(sf::Color::Black);
            hex.setOutlineThickness(1.0f);
            hex.setOrigin( hexSize, hexSize );
            hex.setPosition(middleX, middleY);
            mPoints.push_back(hex);
        }
        n = n->getParent();
    }

    return std::make_pair(cells, cost);
}

