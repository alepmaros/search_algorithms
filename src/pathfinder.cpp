#include "pathfinder.h"

#include <iostream>

Pathfinder::Pathfinder(Map &map)
    : mMap(map)
{
    mGridSize = map.getGridSize();
    mAnimationTimer.restart();

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
    if (foundPath || mCurrentSearchAlgorithm == bs::SearchAlgorithm::None) 
    {
        return;
    }

    if (mCurrentSearchAlgorithm == bs::SearchAlgorithm::BS)
    {
        breadthSearch();
    }
    else if (mCurrentSearchAlgorithm == bs::SearchAlgorithm::DS)
    {
        depthSearch();
    }
    else if (mCurrentSearchAlgorithm == bs::SearchAlgorithm::UCS)
    {
        //uniformCostSearch();
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

    if (mCurrentSearchAlgorithm == bs::SearchAlgorithm::BS)
    {
        breadthSearch();
    }
    else if (mCurrentSearchAlgorithm == bs::SearchAlgorithm::DS)
    {
        depthSearch();
    }
    else if (mCurrentSearchAlgorithm == bs::SearchAlgorithm::UCS)
    {
        //uniformCostSearch();
    }

}

void Pathfinder::initializeSearch()
{
    mLines.clear();
    mPoints.clear();
    mNodesVisited.clear();
    foundPath = false;
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

void Pathfinder::printStatistics(std::string searchAlgo)
{
    std::cout << "----- " << searchAlgo << " Search Statistics -----" << std::endl;
    std::cout << "Start position: (" << mStartPosition.x << "," << 
        mStartPosition.y << ")" << std::endl;
    std::cout << "End position: (" << mEndPosition.x << "," << 
        mEndPosition.y << ")" << std::endl;
    std::cout << "Number of opened nodes: " << mNodes.size() << std::endl;
    std::cout << "Number of cells on path: " << mCellsOnPath << std::endl;
    std::cout << "----- Breadth Search Statistics -----" << std::endl << std::endl;
}

void Pathfinder::breadthSearch()
{
    if (mAnimationTimer.getElapsedTime().asMilliseconds() < 1)
        return;

    while( !mOpen.empty() )
    {
        mNodes.push_back(mOpen.front());
        mOpen.pop_front();
        Node *pe = &mNodes.back();

        sf::Vector2i gridPos = mNodes.back().getGridPos();

        // North
        if (gridPos.y - 1 >= 0)
        {
            if (addNodeBreadth(sf::Vector2i(gridPos.x, gridPos.y-1), pe))
            {
               break; 
            }
        }

        // East
        if (gridPos.x + 1 < mGridSize.x)
        {
            if (addNodeBreadth(sf::Vector2i(gridPos.x+1, gridPos.y), pe))
            {
               break; 
            }
        }

        // South
        if (gridPos.y + 1 < mGridSize.y)
        {
            if (addNodeBreadth(sf::Vector2i(gridPos.x, gridPos.y+1), pe))
            {
               break; 
            }
        }

        // West
        if (gridPos.x - 1 >= 0)
        {
            if (addNodeBreadth(sf::Vector2i(gridPos.x-1, gridPos.y), pe))
            {
               break; 
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

    
    foundPath = true;
    // Move last element
    if ( !mNodes.empty() )
    {
        mNodes.push_back(std::move(mOpen.back()));
        mOpen.pop_back();
        mNodesVisited.clear();
        mCellsOnPath = makePath(&mNodes.back());
        printStatistics("Breadth");
    }
    
}

// addNodeBreadth will return true if the node that it is adding is the destination
bool Pathfinder::addNodeBreadth(sf::Vector2i gridPos, Node *parent)
{
    if ( !mVisited[gridPos.y][gridPos.x] )
    {
        mVisited[gridPos.y][gridPos.x] = true;
        mOpen.push_back(Node(sf::Vector2i(gridPos.x, gridPos.y)));
        mOpen.back().setParent(parent);
        
        // To visualize the visited node
        float blockGap = mMap.getBlockGap();
        float blockSize = mMap.getBlockSize();
        sf::RectangleShape n;
        n.setSize(sf::Vector2f(blockSize, blockSize));
        n.setPosition(mMap.mGrid[gridPos.y][gridPos.x].getPosition());
        n.setFillColor(sf::Color(0,0,0,170));
        mNodesVisited.push_back(n);

        if (mOpen.back().getGridPos() == mEndPosition)
            return true;
    }
    return false;
}

void Pathfinder::depthSearch()
{
    if (mAnimationTimer.getElapsedTime().asMilliseconds() < 1)
        return;

    while( !mOpen.empty() )
    {
        mNodes.push_back(mOpen.front());
        mOpen.pop_front();
        Node *pe = &mNodes.back();

        sf::Vector2i gridPos = mNodes.back().getGridPos();

        // North
        if (gridPos.y - 1 >= 0)
        {
            if (addNodeDepth(sf::Vector2i(gridPos.x, gridPos.y-1), pe))
            {
               break; 
            }
        }

        // East
        if (gridPos.x + 1 < mGridSize.x)
        {
            if (addNodeDepth(sf::Vector2i(gridPos.x+1, gridPos.y), pe))
            {
               break; 
            }
        }

        // South
        if (gridPos.y + 1 < mGridSize.y)
        {
            if (addNodeDepth(sf::Vector2i(gridPos.x, gridPos.y+1), pe))
            {
               break; 
            }
        }

        // West
        if (gridPos.x - 1 >= 0)
        {
            if (addNodeDepth(sf::Vector2i(gridPos.x-1, gridPos.y), pe))
            {
               break; 
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

    
    foundPath = true;
    // Move last element
    if ( !mNodes.empty() )
    {
        mNodes.push_back(std::move(mOpen.front()));
        mOpen.pop_back();

        mNodesVisited.clear();
        mCellsOnPath = makePath(&mNodes.back());
        printStatistics("Depth");
    }
    
}

// addNodeBreadth will return true if the node that it is adding is the destination
bool Pathfinder::addNodeDepth(sf::Vector2i gridPos, Node *parent)
{
    if ( !mVisited[gridPos.y][gridPos.x] )
    {
        mVisited[gridPos.y][gridPos.x] = true;
        mOpen.push_front(Node(sf::Vector2i(gridPos.x, gridPos.y)));
        mOpen.front().setParent(parent);
        
        // To visualize the visited node
        float blockGap = mMap.getBlockGap();
        float blockSize = mMap.getBlockSize();
        sf::RectangleShape n;
        n.setSize(sf::Vector2f(blockSize, blockSize));
        n.setPosition(mMap.mGrid[gridPos.y][gridPos.x].getPosition());
        n.setFillColor(sf::Color(0,0,0,170));
        mNodesVisited.push_back(n);

        if (mOpen.front().getGridPos() == mEndPosition)
            return true;
    }
    return false;
}


int Pathfinder::makePath(Node *dest)
{
    int cells = 1;
    float blockGap = mMap.getBlockGap();
    float blockSize = mMap.getBlockSize();
    float lineThickness = 5.0;
    float hexSize = lineThickness / 1.5;
    Node *n = dest;
    Node *n_next;

    sf::Vector2f lineSize(blockSize + blockGap, lineThickness);
    
    while(n->getParent() != nullptr)
    {
        cells++;

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
   
    return cells;
}
