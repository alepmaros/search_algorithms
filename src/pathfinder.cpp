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
    while( !mOpen.empty() )
    {
        mNodes.push_back(mOpen.front());
        mOpen.pop_front();
        Node *pe = &mNodes.back();

        sf::Vector2i gridPos = mNodes.back().getGridPos();

        // North
        if (gridPos.y - 1 > 0)
        {
            if (addNode(sf::Vector2i(gridPos.x, gridPos.y-1), pe))
            {
               break; 
            }
        }

        // East
        if (gridPos.x + 1 < mGridSize.x)
        {
            if (addNode(sf::Vector2i(gridPos.x+1, gridPos.y), pe))
            {
               break; 
            }
        }

        // South
        if (gridPos.y + 1 < mGridSize.y)
        {
            if (addNode(sf::Vector2i(gridPos.x, gridPos.y+1), pe))
            {
               break; 
            }
        }

        // West
        if (gridPos.x - 1 > 0)
        {
            if (addNode(sf::Vector2i(gridPos.x-1, gridPos.y), pe))
            {
               break; 
            }
        }
    }

    // Move last element
    if ( !mNodes.empty() )
    {
        Node e = mOpen.back();
        mOpen.pop_back();
        mNodes.push_back(std::move(e));
    }
    
    printPath();
}

// addNode will return true if the node that it is adding is the destination
bool Pathfinder::addNode(sf::Vector2i gridPos, Node *parent)
{
    if ( !mVisited[gridPos.y][gridPos.x] )
    {
        mVisited[gridPos.y][gridPos.x] = true;
        mOpen.push_back(Node(sf::Vector2i(gridPos.x, gridPos.y)));
        mOpen.back().setParent(parent);

        if (mOpen.back().getGridPos() == mEndPosition)
            return true;
    }
    return false;
}

void Pathfinder::initializeSearch()
{
    mLines.clear();
    mPoints.clear();
    mStartPosition = mMap.getStartPosition();
    mEndPosition = mMap.getEndPosition();
    mGridSize = mMap.getGridSize();
    
    //std::cout << mStartPosition.x << " " << mStartPosition.y << std::endl;
    //std::cout << mEndPosition.x << " " << mEndPosition.y << std::endl;

    // Add startPosition to node to be visited list
    mOpen.clear();
    mOpen.push_back(Node(mStartPosition));

    mNodes.clear();

    // Clear visited grid
    for (int i = 0; i < mGridSize.y; i++)
    {
        for (int j = 0; j < mGridSize.x; j++)
        {
            mVisited[i][j] = false;
        }
    }
}

void Pathfinder::printPath()
{
    float blockGap = mMap.getBlockGap();
    float blockSize = mMap.getBlockSize();
    float lineThickness = 5.0;
    float hexSize = lineThickness / 1.5;
    Node *n = &mNodes.back();
    Node *n_next;

    sf::Vector2f lineSize(blockSize + blockGap, lineThickness);
    
    while(n->getParent() != nullptr)
    {
        n_next = n->getParent();

        sf::Vector2i nGridPos = n->getGridPos();
        sf::Vector2i nNextGridPos = n_next->getGridPos();

        //std::cout << "(" << n->getGridPos().x << ", " << n->getGridPos().y << ")" << std::endl;

        // Calculate the middle point of the block on pos nGridPos
        float middleX = mMap.mGrid[nGridPos.y][nGridPos.x].getPosition().x + (blockSize/2.0);
        float middleY = mMap.mGrid[nGridPos.y][nGridPos.x].getPosition().y + (blockSize/2.0);

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
       

        n = n->getParent();
    }

    // Adding last hex
    sf::Vector2i nGridPos = n->getGridPos();
    float middleX = mMap.mGrid[nGridPos.y][nGridPos.x].getPosition().x + (blockSize/2.0);
    float middleY = mMap.mGrid[nGridPos.y][nGridPos.x].getPosition().y + (blockSize/2.0);
    sf::CircleShape hex(hexSize, 6);
    hex.setFillColor(sf::Color(0,0,0,0));
    hex.setOutlineColor(sf::Color::Black);
    hex.setOutlineThickness(1.0f);
    hex.setOrigin( hexSize, hexSize );
    hex.setPosition(middleX, middleY);
    mPoints.push_back(hex);
   



}
