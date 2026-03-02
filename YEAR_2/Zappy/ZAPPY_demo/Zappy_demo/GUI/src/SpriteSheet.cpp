/*
** EPITECH PROJECT, 2024
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** SpriteSheet.cpp
*/

#include "SpriteSheet.hpp"

SpriteSheet::SpriteSheet() {
}

SpriteSheet::~SpriteSheet() {
}

bool SpriteSheet::loadFromFile(const std::string& filename) {
    if (!spriteTexture.loadFromFile(filename)) {
        std::cerr << "Erreur: Impossible de charger la feuille de sprites: " << filename << std::endl;
        return false;
    }
    
    initializeSpriteRects();
    std::cout << "Feuille de sprites chargée avec succès: " << filename << std::endl;
    return true;
}

void SpriteSheet::initializeSpriteRects() {
    spriteRects.clear();
    
    sf::Vector2u textureSize = spriteTexture.getSize();
    int cols = textureSize.x / SPRITE_WIDTH;
    int rows = textureSize.y / SPRITE_HEIGHT;
    
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            sf::IntRect rect(
                col * SPRITE_WIDTH,
                row * SPRITE_HEIGHT,
                SPRITE_WIDTH,
                SPRITE_HEIGHT
            );
            spriteRects.push_back(rect);
        }
    }
    
    std::cout << "Initialisé " << spriteRects.size() << " sprites (" << rows << "x" << cols << ")" << std::endl;
}

sf::IntRect SpriteSheet::getSpriteRect(int row, int col) {
    return sf::IntRect(
        col * SPRITE_WIDTH,
        row * SPRITE_HEIGHT,
        SPRITE_WIDTH,
        SPRITE_HEIGHT
    );
}

sf::Sprite SpriteSheet::getSprite(int index) {
    sf::Sprite sprite;
    sprite.setTexture(spriteTexture);
    
    if (index >= 0 && index < static_cast<int>(spriteRects.size())) {
        sprite.setTextureRect(spriteRects[index]);
    } else {
        std::cerr << "Index de sprite invalide: " << index << std::endl;
        if (!spriteRects.empty()) {
            sprite.setTextureRect(spriteRects[0]);
        }
    }
    
    return sprite;
}

sf::Sprite SpriteSheet::getPlayerSprite(int playerLevel, int direction, int animationFrame) {
    sf::Sprite sprite;
    sprite.setTexture(spriteTexture);

    int row = playerLevel - 1;
    int col = (direction * 2) + (animationFrame % 2);
    
    if (row < 0) row = 0;
    if (col >= SPRITES_PER_ROW) col = col % SPRITES_PER_ROW;
    
    sprite.setTextureRect(getSpriteRect(row, col));
    return sprite;
}

sf::Sprite SpriteSheet::getResourceSprite(const std::string& resourceType) {
    sf::Sprite sprite;
    sprite.setTexture(spriteTexture);
    
    int resourceRow = 15;
    int resourceCol = 0;
    
    if (resourceType == "food") {
        resourceRow = 15;
        resourceCol = 0;
    } else if (resourceType == "linemate") {
        resourceRow = 16;
        resourceCol = 0;
    } else if (resourceType == "deraumere") {
        resourceRow = 17;
        resourceCol = 0;
    } else if (resourceType == "sibur") {
        resourceRow = 18;
        resourceCol = 0;
    } else if (resourceType == "mendiane") {
        resourceRow = 19;
        resourceCol = 0;
    } else if (resourceType == "phiras") {
        resourceRow = 20;
        resourceCol = 0;
    } else if (resourceType == "thystame") {
        resourceRow = 21;
        resourceCol = 0;
    }
    
    sprite.setTextureRect(getSpriteRect(resourceRow, resourceCol));
    return sprite;
}

