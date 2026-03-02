/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Look command implementation
*/

#include "server.h"

static void add_players_on_tile(server_t *server, int fd, int x, int y)
{
    int i = 0;
    for (i = 0; i < server->player_count; i++) {
        if (server->players[i] && !server->players[i]->is_dead &&
            server->players[i]->x == x && server->players[i]->y == y) {
            dprintf(fd, " player");
        }
    }
}

static void add_resources_on_tile(server_t *server, int fd, int x, int y)
{
    const char *resource_names[7] = {"food", "linemate", "deraumere",
        "sibur", "mendiane", "phiras", "thystame"};
    map_tile_t *tile = &server->map->tiles[y][x];
    int i = 0;
    int j = 0;
    for (i = 0; i < 7; i++) {
        for (j = 0; j < tile->resources[i]; j++) {
            dprintf(fd, " %s", resource_names[i]);
        }
    }
}

static void calculate_vision_tile(player_t *player, map_t *map, int dist,
                                  int side, int *x, int *y)
{
    *x = player->x;
    *y = player->y;
    switch (player->orientation) {
        case NORTH:
            *y = (*y - dist + map->height) % map->height;
            *x = (*x + side + map->width) % map->width;
            break;
        case EAST:
            *x = (*x + dist) % map->width;
            *y = (*y + side + map->height) % map->height;
            break;
        case SOUTH:
            *y = (*y + dist) % map->height;
            *x = (*x - side + map->width) % map->width;
            break;
        case WEST:
            *x = (*x - dist + map->width) % map->width;
            *y = (*y - side + map->height) % map->height;
            break;
    }
}

void handle_look(server_t *server, int client_index)
{
    player_t *player = get_player_by_fd(server,
                                        server->clients[client_index].fd);
    int fd = server->clients[client_index].fd;
    int vision_range = 0;
    int dist = 0;
    int side = 0;
    if (!player || player->is_dead) {
        dprintf(fd, "dead\n");
        return;
    }
    vision_range = player->level;
    dprintf(fd, "[");
    add_players_on_tile(server, fd, player->x, player->y);
    add_resources_on_tile(server, fd, player->x, player->y);
    for (dist = 1; dist <= vision_range; dist++) {
        for (side = -dist; side <= dist; side++) {
            int x, y;
            calculate_vision_tile(player, server->map, dist, side, &x, &y);
            dprintf(fd, ",");
            add_players_on_tile(server, fd, x, y);
            add_resources_on_tile(server, fd, x, y);
        }
    }
    dprintf(fd, "]\n");
}