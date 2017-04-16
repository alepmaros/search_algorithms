/*
 * Alexandre Maros - 2017/1
 * Blind Search
 *
 */

#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

#include <SFML/Graphics.hpp>
#include "node.h"


class Map
{
    public:
                Map(std::string file, sf::Vector2f offset, float blockSize,
                        float blockGap);

        void            update();
        void            draw(sf::RenderWindow* window);

        // Mark start and end positions
        void            markMap(int x, int y, sf::Mouse::Button buttonPressed);

        sf::Vector2i    getIndexByPosition(int x, int y);

        sf::Vector2i    getGridSize() const;
        sf::Vector2i    getStartPosition() const;
        sf::Vector2i    getEndPosition() const;
        float           getBlockSize() const;
        float           getBlockGap() const;

        std::vector<std::vector<Node> >* getGridPointer();

    private:
        void            setStartPosition(int x, int j);
        void            setEndPosition(int x, int j);
        
    public:
        std::vector<std::vector<Node> > mGrid;

    private:
        // Map offset
        sf::Vector2f mOffset;
        sf::Vector2i mGridSize;

        sf::Vector2i mStartPosition;
        sf::Vector2i mEndPosition;
        sf::RectangleShape mStartRectangle;
        sf::RectangleShape mEndRectangle;

        float mBlockSize;
        float mBlockGap;
};

#endif
