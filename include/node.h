#ifndef NODE_H
#define NODE_H

#include <SFML/Graphics.hpp>

#include "util.h"

class Node
{
    public:
        Node(sf::Vector2i gridPos);
        Node(sf::Vector2i gridPos, int cost);

        void            setParent(Node *parent);

        Node*           getParent() const;
        sf::Vector2i    getGridPos() const;
        int             getCost() const;

    private:
        Node            *mParent;
        sf::Vector2i    mGridPos;
        int             mCost;
};

#endif
