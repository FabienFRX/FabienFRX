/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Game structures and definitions
*/

#ifndef GAME_H_
    #define GAME_H_

    #include <stdbool.h>

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
    int orientation;
    int level;
    int inventory[7];
    double last_food_update;
    double action_start_time;
    double action_end_time;
    bool is_executing_action;
    bool is_dead;
    bool in_incantation;
    char pending_command[256];
    int pending_client_index;
} player_t;

typedef struct map_tile_s {
    int resources[7];
} map_tile_t;

typedef struct map_s {
    int width;
    int height;
    map_tile_t **tiles;
} map_t;

void move_forward(player_t *player, const map_t *map);
void turn_right(player_t *player);
void turn_left(player_t *player);

#endif
