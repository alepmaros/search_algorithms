#ifndef NODE_H
#define NODE_H

#include <SFML/Graphics.hpp>

#include "util.h"

class Node
{
    Node(sf::Vector2i gridPos);

    void    setParent(Node parent);
    
    Node            getParent() const;
    sf::Vector2i    getGridPos() const;
    int             getCost() const;

    private:
        Node            *parent;
        sf::Vector2i    mGridPos;
};

#endif
