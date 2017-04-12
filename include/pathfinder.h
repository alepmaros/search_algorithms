#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <SFML/Graphics.hpp>
#include <list>

#include "util.h"
#include "node.h"
#include "map.h"

class Pathfinder
{
    public:
        Pathfinder(Map &map);

        void    draw(sf::RenderWindow *window);
        void    update();

        void    setSearchAlgorithm(bs::SearchAlgorithm sa);
        void    calculatePath(bs::SearchAlgorithm sa);

        void    breadthSearch();

        void    printPath();

    private:
        void    initializeSearch();
        bool    addNode(sf::Vector2i gridPos, Node *parent);

    private:
        Map                             &mMap;
        bs::SearchAlgorithm             mCurrentSearchAlgorithm;

        // Variables used in the algorithms
        // mVisited = stores if position was already visited
        // mOpen = list of nodes yet to be visited
        // mNodes = vector to store the nodes already visited
        std::vector<std::vector<bool> > mVisited;
        std::list<Node>                 mOpen;
        std::list<Node>                 mNodes;
        sf::Vector2i                    mStartPosition;
        sf::Vector2i                    mEndPosition;
        sf::Vector2i                    mGridSize;

        std::vector<sf::CircleShape>    mPoints;
        std::vector<sf::RectangleShape> mLines;
        std::vector<sf::RectangleShape> mNodesVisited;

        // Control Variables

};

#endif
