/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Player movement functions
*/

#include "server.h"

void move_forward(player_t *player, const map_t *map)
{
    if (!player || !map)
        return;
    switch (player->orientation) {
        case NORTH:
            player->y = (player->y - 1 + map->height) % map->height;
            break;
        case EAST:
            player->x = (player->x + 1) % map->width;
            break;
        case SOUTH:
            player->y = (player->y + 1) % map->height;
            break;
        case WEST:
            player->x = (player->x - 1 + map->width) % map->width;
            break;
    }
}

void turn_left(player_t *player)
{
    if (!player)
        return;
    player->orientation = ((player->orientation - 2) % 4) + 1;
}

void turn_right(player_t *player)
{
    if (!player)
        return;
    player->orientation = (player->orientation % 4) + 1;
}
