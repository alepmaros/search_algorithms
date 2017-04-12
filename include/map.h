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


class Map
{
    public:
                Map(std::string file, sf::Vector2f offset, float blockSize,
                        float blockGap);

        void    update();
        void    draw(sf::RenderWindow* window);
        
    public:
        std::vector<std::vector<sf::RectangleShape> > mGrid;

    private:
        // Map offset
        sf::Vector2f mOffset;

        float mBlockSize;
        float mBlockGap;
        
        int mGridWidth;
        int mGridHeight;

};

#endif
