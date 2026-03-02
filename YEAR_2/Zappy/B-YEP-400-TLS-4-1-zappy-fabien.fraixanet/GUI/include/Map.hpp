/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Map.hpp
*/

#ifndef MAP_HPP
#define MAP_HPP
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <map>
#include "Player.hpp"
#include <memory>

class Map {
    public:
        void initialize(int width, int height);
        void draw(sf::RenderWindow& window, int tileSize, int offsetX, int offsetY, sf::Time elapsed);
        int getWidth() const;
        int getHeight() const;
        void addPlayer(int id, int x, int y, PlayerOrientation orientation, int level);
        void updatePlayerPosition(int id, int x, int y, PlayerOrientation orientation);
        sf::Texture& getPlayerTexture(int level, PlayerOrientation orientation);
        std::map<std::string, sf::Texture> resourceTextures;
        void setResourcesAt(int x, int y, const std::vector<int>& resources);
        const std::vector<std::string> resourceNames = {
        "food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"
        };
        void removePlayer(int id);
        
    private:
        int width = 0;
        int height = 0;
        sf::Texture cellTexture;
        std::vector<sf::Texture> textures;
        std::vector<std::pair<int, int>> buildingPositions;
        std::vector<int> buildingIndices;
        std::map<int, std::unique_ptr<Player>> _players;
        void loadResourceTextures();
        sf::Font font;
        std::map<std::pair<int, int>, std::vector<int>> mapResources;
        std::map<std::string, sf::Vector2f> resourceOffsets;
        std::map<int, std::map<PlayerOrientation, sf::Texture>> _playerTextures;
        sf::Texture _defaultTexture;
        void loadPlayerTextures();
    };

#endif