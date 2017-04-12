/*
 * Alexandre Maros - 2017/1
 * Blind Search
 *
 */

#include <fstream>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "map.h"
#include "util.h"

Map::Map(std::string file, sf::Vector2f offset, float blockSize, float blockGap)
    : mOffset(offset)
    , mBlockSize(blockSize)
    , mBlockGap(blockGap)
{
    std::string line;
    std::ifstream infile(file);

    if (!infile.is_open())
    {
        std::cout << "Error! Map file not found" << std::endl;
        std::exit(-1);
    }

    int i = 0, j = 0;
    // Read map
    while(getline(infile, line))
    {
        mGrid.push_back(std::vector<sf::RectangleShape>());
        int type;
        std::istringstream ss(line);
        while(ss >> type)
        {
            std::cout << type << " ";

            sf::RectangleShape rs;
            rs.setSize(sf::Vector2f(blockSize, blockSize));
            rs.setPosition( sf::Vector2f((j+1) * blockGap + j * blockSize,
                        (i+1) * blockGap + i * blockSize) );
            
            switch(type)
            {
                case bs::FloorType::Grass:
                    rs.setFillColor(sf::Color::Green);
                    break;
                case bs::FloorType::Hill:
                    rs.setFillColor(sf::Color::Yellow);
                    break;
                case bs::FloorType::Swamp:
                    rs.setFillColor(sf::Color::Black);
                    break;
                case bs::FloorType::Mountain:
                    rs.setFillColor(sf::Color::Red);
                    break;
            }
            mGrid[i].push_back(rs);
            j++;
        }
        i++;
        j=0;
        std::cout << std::endl;
    }

}

void Map::update()
{
    // There is nothing to be updated (not sure if it will in the future)
    return;
}

void Map::draw(sf::RenderWindow* window)
{
    std::vector<std::vector<sf::RectangleShape> >::iterator it_i;
    std::vector<sf::RectangleShape>::iterator it_j;

    for (it_i = mGrid.begin(); it_i != mGrid.end(); it_i++)
    {
        for (it_j = it_i->begin(); it_j != it_i->end(); it_j++)
        {
            window->draw(*it_j);
        }
    }
}
