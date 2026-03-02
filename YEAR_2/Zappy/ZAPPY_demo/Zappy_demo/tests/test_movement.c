/*
** EPITECH PROJECT, 2025
** B-YEP-Zappy
** File description:
** test_movement.c
*/

#ifndef GAME_H_
#define GAME_H_

#include "server.h"

typedef enum orientation_e {
    NORTH = 1,
    EAST = 2,
    SOUTH = 3,
    WEST = 4
} orientation_t;

typedef struct player_s {
    int id;
    int x;
    int y;
    orientation_t orientation;
    int level;
} player_t;

typedef struct map_s {
    int width;
    int height;
} map_t;


void move_forward(player_t *player, const map_t *map);
void turn_right(player_t *player);
void turn_left(player_t *player);


#endif /* !GAME_H_ */
