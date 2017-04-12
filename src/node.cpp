#include "node.h"

Node::Node(sf::Vector2i gridPos)
    : mGridPos(gridPos)
{
    mParent = nullptr;
    mCost = 0;
}

Node::Node(sf::Vector2i gridPos, int cost)
    : Node(gridPos)
{
    mCost = cost;
}

void Node::setParent(Node *parent)
{
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
