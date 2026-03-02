/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Mar, 2025                                                     *
 * Title           - B-OOP-400-TLS-4-1-arcade-nolan.dumoulin [WSL: Debian]              *
 * Description     -                                                                    *
 *     SFMLModule                                                                       *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _|           *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _|           *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_|           *
 *       _|        _|          _|        _|      _|        _|        _|    _|           *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _|           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#pragma once

#include "../interfaces/ADisplayModule.hpp"
#include "../utils/ResourceManager.hpp"
#include "../exceptions/DisplayException.hpp"
#include <cmath>
#include <filesystem>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

namespace arcade {
    class SFMLModule : public ADisplayModule {
    private:
        sf::RenderWindow *window;
        sf::Font *font;
        ResourceManager *resourceManager;
        sf::Color convertColor(const Color &color);

    public:
        SFMLModule();
        ~SFMLModule() override;

        void init() override;
        void stop() override;
        void clear() override;
        void display() override;
        bool pollEvent(InputEvent &event) override;
        bool isOpen() const override;
        void drawRect(int x, int y, int width, int height, const Color &color) override;
        void drawCircle(int x, int y, int radius, const Color &color) override;
        void drawText(int x, int y, const std::string &text, const Color &color) override;

        void drawLine(int x1, int y1, int x2, int y2, const Color &color);
        void drawRectOutline(int x, int y, int width, int height, const Color &color, int outlineThickness = 1);
        void drawCircleOutline(int x, int y, int radius, const Color &color, int outlineThickness = 1);
        void drawSprite(int x, int y, const std::string &texturePath, int width = 0, int height = 0);
    };

    extern "C" {
        IDisplayModule *createDisplayModule();
    }
}
