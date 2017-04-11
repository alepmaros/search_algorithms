/*
 * Alexandre Maros - 2017/1
 * Blind Search
 *
 */

#include <fstream>
#include <cstdlib>
#include <iostream>

#include "map.h"

Map::Map(std::string file)
{
    std::ifstream infile(file);

    if (!infile.is_open())
    {
        std::cout << "Error! Map file not found" << std::endl;
        std::exit(-1);
    }

    // Read map

}

void Map::update()
{
    // There is nothing to be updated (not sure if it will in the future)
    return;
}

void Map::draw(sf::RenderWindow* window)
{

}
