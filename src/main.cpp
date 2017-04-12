/*
 * Alexandre Maros - 2017/1
 * Blind Search
 *
 */

#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include <SFML/Graphics.hpp>

#include "map.h"

#define BLOCK_SIZE 15
#define BLOCK_GAP 1
#define X_MAP_OFFSET 0
#define Y_MAP_OFFSET 0

int main()
{
    Map m("robo_ambiente.txt", sf::Vector2f(X_MAP_OFFSET, Y_MAP_OFFSET), BLOCK_SIZE,
            BLOCK_GAP);

    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        m.draw(&window);
        window.display();
    }

    return 0;
}

