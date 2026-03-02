/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sun, Feb, 2025                                                     *
 * Title           - B-PDG-300-TLS-3-1-PDGRUSH3-sven.reichert [WSL: Debian]             *
 * Description     -                                                                    *
 *     SfmlDisplay                                                                      *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░             *
 *         ▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓             *
 *         █  ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "../include/SfmlDisplay.hpp"

Krell::SFMLDisplay::SFMLDisplay() : window(sf::VideoMode(800, 600), "Krell")
{
    window.setFramerateLimit(60);
    try {
        window.setVerticalSyncEnabled(true);
    } catch (const std::exception &e) {
        std::cerr << "Setting vertical sync not supported: " << e.what() << std::endl;
    }
    if (!window.isOpen()) {
        throw std::runtime_error("Failed to open window!");
    }
}

Krell::SFMLDisplay::~SFMLDisplay()
{
    window.close();
}

void Krell::SFMLDisplay::draw(const std::string &data)
{
    if (data.empty()) {
        return;
    } else {
        window.clear();
        sf::Font font;
        if (!font.loadFromFile("./assets/arial.ttf")) {
            throw std::runtime_error("Failed to load font!");
        }
        sf::Text text(data, font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        window.draw(text);
        window.display();
    }
}

sf::RenderWindow &Krell::SFMLDisplay::getWindow()
{
    return window;
}