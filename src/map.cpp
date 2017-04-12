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
            //std::cout << type << " ";

            sf::RectangleShape rs;
            rs.setSize(sf::Vector2f(blockSize, blockSize));
            rs.setPosition( sf::Vector2f((j+1) * blockGap + j * blockSize + offset.x,
                        (i+1) * blockGap + i * blockSize + offset.y) );
            
            switch(type)
            {
                case bs::FloorType::Grass:
                    rs.setFillColor(sf::Color::Green);
                    break;
                case bs::FloorType::Hill:
                    rs.setFillColor(sf::Color(128, 73, 2));
                    break;
                case bs::FloorType::Swamp:
                    rs.setFillColor(sf::Color(32, 63, 255));
                    break;
                case bs::FloorType::Fire:
                    rs.setFillColor(sf::Color(255, 57, 4));
                    break;
            }
            mGrid[i].push_back(rs);
            j++;
        }
        mGridSize.x = j;
        i++;
        j=0;
        //std::cout << std::endl;
    }
    mGridSize.y = i;

    setStartPosition(3,3);
    setEndPosition(30,30);
}

void Map::update()
{
    // There is nothing to be updated (not sure if it will in the future)
    return;
}

void Map::draw(sf::RenderWindow* window)
{
    std::vector<std::vector<sf::RectangleShape> >::iterator itvv;
    std::vector<sf::RectangleShape>::iterator itv;

    for (itvv = mGrid.begin(); itvv != mGrid.end(); itvv++)
    {
        for (itv = itvv->begin(); itv != itvv->end(); itv++)
        {
            window->draw(*itv);
        }
    }

    window->draw(mStartRectangle);
    window->draw(mEndRectangle);
}

sf::Vector2i Map::getGridSize()
{
    return mGridSize;
}

// x and y represent the index of the vector
void Map::setStartPosition(int x, int y)
{
   mStartRectangle.setPosition(sf::Vector2f((x+1) * mBlockGap + x * mBlockSize + mOffset.x,
                        (y+1) * mBlockGap + y * mBlockSize + mOffset.y));
   mStartRectangle.setSize(sf::Vector2f(mBlockSize, mBlockSize));
   mStartRectangle.setFillColor(sf::Color(0,0,0,0));
   mStartRectangle.setOutlineThickness(5);
   mStartRectangle.setOutlineColor(sf::Color(255, 0, 180));

   mStartPosition = sf::Vector2i(x,y);
}

// x and y represent the index of the vector
void Map::setEndPosition(int x, int y)
{
   mEndRectangle.setPosition(sf::Vector2f((x+1) * mBlockGap + x * mBlockSize + mOffset.x,
                        (y+1) * mBlockGap + y * mBlockSize + mOffset.y));
   mEndRectangle.setSize(sf::Vector2f(mBlockSize, mBlockSize));
   mEndRectangle.setFillColor(sf::Color(0,0,0,0));
   mEndRectangle.setOutlineThickness(5);
   mEndRectangle.setOutlineColor(sf::Color(255,255,0));

   mEndPosition = sf::Vector2i(x,y);
}

// Get the index of the Tile based on a Screen position
sf::Vector2i Map::getIndexByPosition(int x, int y)
{
    std::vector<std::vector<sf::RectangleShape> >::iterator itvv;
    std::vector<sf::RectangleShape>::iterator itv;
    int i = 0, j = 0;

    for (itv = mGrid.at(0).begin(); itv != mGrid.at(0).end(); itv++)
    {
        sf::Vector2f pos = itv->getPosition();
        if (x > pos.x && x < pos.x+mBlockSize)
        {
            break;
        } 
        i++;
    }

    for (itvv = mGrid.begin(); itvv != mGrid.end(); itvv++)
    {
        sf::Vector2f pos = itvv->at(0).getPosition();
        if (y > pos.y && y < pos.y+mBlockSize)
        {
            break;
        }
        j++;
    }

    return sf::Vector2i(i,j);
}

void Map::markMap(int x, int y, sf::Mouse::Button buttonPressed)
{
    sf::Vector2i index = getIndexByPosition(x, y);

    // If it does not belong in the map
    if ( index.x == mGridSize.x || index.y == mGridSize.y)
        return;

    if ( buttonPressed == sf::Mouse::Left)
    {
        // start position cant be equal to end position
        if (mEndPosition.x != index.x || mEndPosition.y != index.y)
            setStartPosition(index.x, index.y);
    }
    else if ( buttonPressed == sf::Mouse::Right )
    {
        // end position cant be equal to start position
        if (mStartPosition.x != index.x || mStartPosition.y != index.y)
            setEndPosition(index.x, index.y);
    }
}
