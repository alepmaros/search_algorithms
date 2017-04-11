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
        enum FloorType
        {
            None,
            Grass,
            Hill,
            Swamp,
            Mountains
        };

    public:
                Map(std::string file);

        void    update();
        void    draw(sf::RenderWindow* window);
        
    public:
        std::vector<std::vector<FloorType> > mGrid;

    private:
        float xOffset;
        float yOffset;
        
        int mGridWidth;
        int mGridHeight;

};

#endif
