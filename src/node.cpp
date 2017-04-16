#include "node.h"

Node::Node(sf::Vector2i gridPos, bs::FloorType type, sf::Vector2f blockPosition,
        sf::Vector2f size)
    : mGridPos(gridPos)
      , mType(type)
{
    mParent = nullptr;

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
