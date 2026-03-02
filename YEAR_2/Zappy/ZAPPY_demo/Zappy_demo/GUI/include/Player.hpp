/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Player.hpp
*/

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

enum class PlayerOrientation {
    North = 1, East = 2, South = 3, West = 4
};

class Map;

class Player {
    public:
        Player(int id, int x, int y, PlayerOrientation orientation, int level);

        void setPosition(int x, int y);
        void setOrientation(PlayerOrientation orientation);
        void setLevel(int level);
        void update(sf::Time elapsed);
        void draw(sf::RenderWindow& window, Map& map, float tile_size, float offset_x, float offset_y);

        int getLevel() const;
        PlayerOrientation getOrientation() const;

    private:
        int _id;
        int _level;
        sf::Vector2i _mapPosition;
        PlayerOrientation _orientation;
};

#endif