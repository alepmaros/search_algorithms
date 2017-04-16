#ifndef NODE_H
#define NODE_H

#include <SFML/Graphics.hpp>

#include "util.h"

class Node : public sf::Drawable
{
    public:
        Node(sf::Vector2i gridPos, bs::FloorType type, 
                sf::Vector2f position, sf::Vector2f size);

        void            setParent(Node *parent);

        Node*           getParent() const;
        sf::Vector2i    getGridPos() const;
        sf::Vector2f    getPosition() const;
        int             getCost() const;

    private:
        virtual void    draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        Node                *mParent;
        sf::Vector2i        mGridPos;
        int                 mCost;
        sf::RectangleShape  mShape;
        bs::FloorType       mType;
};

#endif
