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

#include "node.h"

Node::Node(sf::Vector2i gridPos, bs::FloorType type, sf::Vector2f blockPosition,
        sf::Vector2f size)
    : mGridPos(gridPos)
      , mType(type)
{
    mParent = nullptr;
    mPathCost = 0;
    mWasVisited = false;
    mAdjacencyList.clear();

    mShape.setPosition(blockPosition);
    mShape.setSize(size);
    switch(type)
    {
        case bs::FloorType::Grass:
            mShape.setFillColor(sf::Color::Green);
            mCost = 1;
            break;
        case bs::FloorType::Hill:
            mShape.setFillColor(sf::Color(128, 73, 2));
            mCost = 5;
            break;
        case bs::FloorType::Swamp:
            mShape.setFillColor(sf::Color(32, 63, 255));
            mCost = 10;
            break;
        case bs::FloorType::Fire:
            mShape.setFillColor(sf::Color(255, 57, 4));
            mCost = 15;
            break;
    }
}

void Node::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mShape);
}

void Node::setParent(Node *parent)
{
    if (parent != nullptr)
    {
        mPathCost = parent->getPathCost();
        mLevel = parent->getLevel() + 1;
    }
    else
    {
        mPathCost = 0;
        mLevel = 0;
        mPossiblePathCost = 0;
        mWasVisited = false;
    }

    mParent = parent;
}

Node* Node::getParent() const
{
    return mParent;
}

sf::Vector2i Node::getGridPos() const
{
    return mGridPos;
}

int Node::getCost() const
{
    return mCost;
}

sf::Vector2f Node::getPosition() const
{
    return mShape.getPosition();
}

int Node::getPathCost() const
{
    return mPathCost + mCost;
}

int Node::addPathCost(int cost)
{
    mPathCost += cost;
    return mPathCost;
}

void Node::setPathCost(int cost)
{
    mPathCost = cost;
}

bool Node::wasVisited()
{
    return mPathCost != 0 || mWasVisited;
}

void Node::setLevel(int level)
{
    mLevel = level;
}

int Node::getLevel() const
{
    return mLevel;
}

std::vector<Node*> Node::getAdjacentNodes()
{
    return mAdjacencyList;
}

void Node::addAdjacentNode(Node *n)
{
    mAdjacencyList.push_back(n);
}

void Node::setVisited()
{
    mWasVisited = true;
}

float Node::getPossiblePathCost() const
{
    return mPossiblePathCost;
}

void Node::setPossiblePathCost(float g)
{
    mPossiblePathCost = g;
}
