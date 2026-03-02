/*
** EPITECH PROJECT, 2024
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** SpriteSheet.hpp
*/

#ifndef SPRITESHEET_HPP
#define SPRITESHEET_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class SpriteSheet {
    public:
        SpriteSheet();
        ~SpriteSheet();
        
        bool loadFromFile(const std::string& filename);
        sf::Sprite getSprite(int index);
        sf::Sprite getPlayerSprite(int playerLevel, int direction, int animationFrame = 0);
        sf::Sprite getResourceSprite(const std::string& resourceType);
        static const int SPRITE_WIDTH = 32;
        static const int SPRITE_HEIGHT = 32;
        static const int SPRITES_PER_ROW = 7;
        enum ResourceType {
            FOOD = 0,
            LINEMATE,
            DERAUMERE,
            SIBUR,
            MENDIANE,
            PHIRAS,
            THYSTAME
        };
        
    private:
        sf::Texture spriteTexture;
        std::vector<sf::IntRect> spriteRects;
        
        void initializeSpriteRects();
        sf::IntRect getSpriteRect(int row, int col);
};

#endif

