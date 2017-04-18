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
#include "pathfinder.h"
#include "util.h"

#define BLOCK_SIZE 16
#define BLOCK_GAP 1
#define X_MAP_OFFSET 0
#define Y_MAP_OFFSET 50
#define FONT_SIZE 20

int main()
{
    Map m("robo_ambiente.txt", sf::Vector2f(X_MAP_OFFSET, Y_MAP_OFFSET), BLOCK_SIZE,
            BLOCK_GAP);

    sf::Vector2i gridSize = m.getGridSize();

    int sizeWindowY = Y_MAP_OFFSET + ( (gridSize.y+1) * BLOCK_GAP) + (BLOCK_SIZE * gridSize.y);
    int sizeWindowX = X_MAP_OFFSET + ( (gridSize.x+1) * BLOCK_GAP) + (BLOCK_SIZE * gridSize.x);
    sizeWindowX *= 2;
    
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(sizeWindowX, sizeWindowY), "Blind Search", 
            sf::Style::Default, settings);

    // remove this later
    //window.setFramerateLimit(60);

    // Variables to calculate the FPS
    sf::Clock clock;
    float lastTime;

    sf::Font font;
    font.loadFromFile("fonts/Roboto-Black.ttf");

    sf::Text textFps("", font);
    textFps.setFillColor(sf::Color::White);
    textFps.setCharacterSize(FONT_SIZE);
    textFps.setPosition(sf::Vector2f(2,0));

    sf::Text textSelectedSearch("Press 1-3 to select Search Algorithm", font);
    textSelectedSearch.setFillColor(sf::Color::White);
    textSelectedSearch.setCharacterSize(FONT_SIZE);
    textSelectedSearch.setPosition(sf::Vector2f(2,22));

    Pathfinder pathfinder(m);
    bs::SearchAlgorithm selectedSearch = bs::SearchAlgorithm::None;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                m.markMap(event.mouseButton.x, event.mouseButton.y, event.mouseButton.button);
            }

            if (event.type == sf::Event::KeyPressed)
            {
                switch(event.key.code)
                {
                    case sf::Keyboard::Num1:
                        textSelectedSearch.setString("Breadth Search");
                        selectedSearch = bs::SearchAlgorithm::BFS;
                        break;
                    case sf::Keyboard::Num2:
                        textSelectedSearch.setString("Depth Search");
                        selectedSearch = bs::SearchAlgorithm::DFS;
                        break;
                    case sf::Keyboard::Num3:
                        textSelectedSearch.setString("Uniform Cost Search");
                        selectedSearch = bs::SearchAlgorithm::UCS;
                        break;
                    case sf::Keyboard::Num4:
                        textSelectedSearch.setString("Iterative Deepening Search");
                        selectedSearch = bs::SearchAlgorithm::IDS;
                        break;
                    case sf::Keyboard::Return:
                        pathfinder.calculatePath(selectedSearch);
                        break;
                }
            }
        }

        pathfinder.update();

        // Calculate Frames per Second
        float currentTime = clock.restart().asSeconds();
        int fps = 1.f / lastTime;
        lastTime = currentTime;

        // Set the current FPS
        textFps.setString(std::string("FPS: " + std::to_string(fps)));

        window.clear();
        window.draw(textFps);
        m.draw(&window);
        pathfinder.draw(&window);
        window.draw(textSelectedSearch);
        window.display();
    }

    return 0;
}

