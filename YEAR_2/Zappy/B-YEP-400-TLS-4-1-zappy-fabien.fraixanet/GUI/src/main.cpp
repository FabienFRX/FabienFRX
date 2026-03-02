/*
** EPITECH PROJECT, 2024
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** main.cpp
*/

#include <SFML/Audio.hpp>
#include "Client.hpp"
#include "Map.hpp"
#include "Error.hpp"
#include "Init.hpp"

int playMusic(sf::Music& music)
{
    if (!music.openFromFile("GUI/assets/music/son.mp3")) {
        std::cerr << "Error loading music file!" << std::endl;
        return 84;
    }
    music.setLoop(true);
    music.play();
    return 0;
}

int main(int argc, char** argv)
{
    Map map;
    Client client;
    int port;
    std::string host;
    sf::Music music;

    if (!checkError(argc, argv, map, client, port, host)) {
        return 1;
    }
    
    std::cout << "Connexion au serveur établie, création de la fenêtre..." << std::endl;
    
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Zappy GUI");
    const int size_square = 96;
    float gridWidth = (map.getWidth() + map.getHeight()) * (size_square / 2.0f);
    float gridHeight = (map.getWidth() + map.getHeight()) * (size_square / 4.0f);
    float size_x = (1000 - gridWidth) / 2.0f + ((map.getHeight() - 1) * (size_square / 2.0f));
    float size_y = (1000 - gridHeight) / 2.0f;

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("GUI/assets/background.png")) {
        std::cerr << "Erreur: Impossible de charger background.png" << std::endl;
        return 1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(
        float(1000) / backgroundTexture.getSize().x,
        float(1000) / backgroundTexture.getSize().y
    );
    
    sf::Clock clock;
    playMusic(music);
    
    std::cout << "Fenêtre créée, boucle principale..." << std::endl;
    
    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        client.processServerCommands(map);
        window.clear();
        window.draw(backgroundSprite);
        map.draw(window, size_square, size_x, size_y, elapsed);
        window.display();
    }
    
    std::cout << "GUI fermé." << std::endl;
    return 0;
}
