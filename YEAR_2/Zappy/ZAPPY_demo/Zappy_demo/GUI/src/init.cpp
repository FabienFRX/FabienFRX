/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** init.cpp
*/

#include "Init.hpp"
#include "Player.hpp"

void initWindows(Map& map, int size_square, int windowWidth, int windowHeight)
{
    float gridWidth = (map.getWidth() + map.getHeight()) * (size_square / 2.0f);
    float gridHeight = (map.getWidth() + map.getHeight()) * (size_square / 4.0f);

    const int decale = 1;
    float size_x = (windowWidth - gridWidth) / 2.0f + ((map.getHeight() - decale) * (size_square / 2.0f));
    float size_y = (windowHeight - gridHeight) / 2.0f;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Zappy GUI");
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("GUI/assets/background.png")) {
        std::cerr << "Failed to load background image" << std::endl;
        return;
    }
    sf::Sprite backgroundSprite(backgroundTexture);

    sf::Vector2u textureSize = backgroundTexture.getSize();
    backgroundSprite.setScale(
        float(windowWidth) / textureSize.x,
        float(windowHeight) / textureSize.y
    );

    // map.addPlayer(1, 2, 2, PlayerOrientation::South, 1);
    // map.addPlayer(2, 5, 8, PlayerOrientation::East, 1);
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        window.draw(backgroundSprite);
        map.draw(window, size_square, size_x, size_y, elapsed);
        window.display();
    }
}
