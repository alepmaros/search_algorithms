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
        void            setLevel(int level);

        Node*           getParent() const;
        sf::Vector2i    getGridPos() const;
        sf::Vector2f    getPosition() const;
        int             getCost() const;
        int             getPathCost() const;
        int             getLevel() const;

        int             addPathCost(int cost);
        void            setPathCost(int cost);

        bool            wasVisited();
        void            setVisited();

        std::vector<Node*> getAdjacentNodes();
        void            addAdjacentNode(Node *n);

    private:
        virtual void    draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        std::vector<Node*>  mAdjacencyList;
        Node                *mParent;
        sf::Vector2i        mGridPos;

        int                 mCost;
        int                 mPathCost;
        int                 mLevel;

        bool                mWasVisited;

        sf::RectangleShape  mShape;
        bs::FloorType       mType;
};

#endif
