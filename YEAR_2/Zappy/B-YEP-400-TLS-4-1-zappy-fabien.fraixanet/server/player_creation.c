/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Player creation functions
*/

#define _POSIX_C_SOURCE 200809L
#include "server.h"
#include <time.h>

static void init_player_stats(player_t *player, int fd, int x, int y)
{
    int k = 0;

    player->id = fd;
    player->level = 1;
    player->x = x;
    player->y = y;
    player->orientation = 1 + (rand() % 4);
    for (k = 0; k < 7; k++)
        player->inventory[k] = 0;
    player->inventory[0] = 10;
}

static void init_player_time_fields(player_t *player)
{
    player->now = get_current_time();
    player->last_time = player->now;
    player->timeout = convert_timeout(126.0 / 100);
    player->action_end_time = 0.0;
    player->is_executing_action = false;
    player->is_dead = false;
    player->in_incantation = false;
    memset(player->pending_command, 0, sizeof(player->pending_command));
    player->pending_client_index = -1;
}

player_t *create_player_direct(server_t *server, int client_fd,
                               const char *team_name)
{
    player_t *new_player = malloc(sizeof(player_t));
    int x = 0;
    int y = 0;

    (void)team_name;
    if (!new_player)
        return NULL;
    x = rand() % server->args->width;
    y = rand() % server->args->height;
    init_player_stats(new_player, client_fd, x, y);
    init_player_time_fields(new_player);
    return new_player;
}

player_t *create_player_from_egg(server_t *server, int client_fd,
                                 const char *team_name)
{
    egg_t *oldest_egg = find_oldest_egg(server, team_name);
    player_t *new_player = NULL;

    if (!oldest_egg)
        return NULL;
    oldest_egg->is_hatched = true;
    new_player = malloc(sizeof(player_t));
    if (!new_player)
        return NULL;
    init_player_stats(new_player, client_fd, oldest_egg->x, oldest_egg->y);
    init_player_time_fields(new_player);
    return new_player;
}
