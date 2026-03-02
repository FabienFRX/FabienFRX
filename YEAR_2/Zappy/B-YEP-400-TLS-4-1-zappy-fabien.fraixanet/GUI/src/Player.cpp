/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Player.cpp
*/

#include "Player.hpp"
#include "Map.hpp"
#include <iostream>

Player::Player(int id, int x, int y, PlayerOrientation orientation, int level)
    : _id(id), _level(level), _mapPosition(x, y), _orientation(orientation) {}

void Player::update(sf::Time /*elapsed*/) {}

void Player::setPosition(int x, int y)
{
    _mapPosition.x = x;
    _mapPosition.y = y;
}

void Player::setOrientation(PlayerOrientation orientation)
{
    _orientation = orientation;
}

void Player::setLevel(int level)
{
    _level = level;
}

int Player::getLevel() const
{
    return _level;
}

PlayerOrientation Player::getOrientation() const
{
    return _orientation;
}

void Player::draw(sf::RenderWindow& window, Map& map, float tile_size, float offset_x, float offset_y)
{
    sf::Texture& texture = map.getPlayerTexture(_level, _orientation);
    sf::Sprite sprite(texture);

    sprite.setScale(1.5f, 1.5f);
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();

    float tile_center_x = (_mapPosition.x - _mapPosition.y) * (tile_size / 2.0f) + offset_x + (tile_size / 2.0f);
    float tile_center_y = (_mapPosition.x + _mapPosition.y) * (tile_size / 4.0f) + offset_y + (tile_size / 4.0f);

    float final_x = tile_center_x - (spriteBounds.width / 2.0f);
    float final_y = tile_center_y - spriteBounds.height;

    sprite.setPosition(final_x, final_y);
    window.draw(sprite);
}