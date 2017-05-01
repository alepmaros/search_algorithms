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
        
        float           getPossiblePathCost() const;
        void            setPossiblePathCost(float g);
        

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

        // used for A* search
        float               mPossiblePathCost;

        bool                mWasVisited;

        sf::RectangleShape  mShape;
        bs::FloorType       mType;
};

#endif
