/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Resource manipulation commands
*/

#include "server.h"

static int get_resource_index(const char *resource_name)
{
    const char *resources[7] = {"food", "linemate", "deraumere", "sibur",
                                "mendiane", "phiras", "thystame"};
    int i = 0;

    for (i = 0; i < 7; i++) {
        if (strcasecmp(resource_name, resources[i]) == 0)
            return i;
    }
    return -1;
}

void handle_take(server_t *server, int client_index, const char *cmd)
{
    player_t *player = get_player_by_fd(server,
                                        server->clients[client_index].fd);
    char resource_name[64] = {0};
    int resource_index = 0;
    map_tile_t *tile = NULL;

    if (!player || player->is_dead) {
        dprintf(server->clients[client_index].fd, "dead\n");
        return;
    }
    if (sscanf(cmd, "Take %63s", resource_name) != 1 &&
        sscanf(cmd, "take %63s", resource_name) != 1) {
        dprintf(server->clients[client_index].fd, "ko\n");
        return;
    }
    resource_index = get_resource_index(resource_name);
    if (resource_index == -1) {
        dprintf(server->clients[client_index].fd, "ko\n");
        return;
    }
    tile = &server->map->tiles[player->y][player->x];
    if (tile->resources[resource_index] > 0) {
        tile->resources[resource_index]--;
        player->inventory[resource_index]++;
        dprintf(server->clients[client_index].fd, "ok\n");
    } else {
        dprintf(server->clients[client_index].fd, "ko\n");
    }
}

void handle_set(server_t *server, int client_index, const char *cmd)
{
    player_t *player = get_player_by_fd(server,
                                        server->clients[client_index].fd);
    char resource_name[64];
    int resource_index = 0;
    map_tile_t *tile = NULL;

    if (!player || player->is_dead) {
        dprintf(server->clients[client_index].fd, "dead\n");
        return;
    }
    if (sscanf(cmd, "Set %s", resource_name) != 1) {
        dprintf(server->clients[client_index].fd, "ko\n");
        return;
    }
    resource_index = get_resource_index(resource_name);
    if (resource_index == -1) {
        dprintf(server->clients[client_index].fd, "ko\n");
        return;
    }
    if (player->inventory[resource_index] > 0) {
        tile = &server->map->tiles[player->y][player->x];
        player->inventory[resource_index]--;
        tile->resources[resource_index]++;
        dprintf(server->clients[client_index].fd, "ok\n");
    } else {
        dprintf(server->clients[client_index].fd, "ko\n");
    }
}

void handle_connect_nbr(server_t *server, int client_index)
{
    char *team_name = server->clients[client_index].team_name;
    int available_eggs = count_team_eggs(server, team_name);
    int active_players = count_team_players(server, team_name);
    int total_possible_players = server->args->clients_nb + available_eggs;
    int available_slots = total_possible_players - active_players;

    if (available_slots < 0)
        available_slots = 0;
    dprintf(server->clients[client_index].fd, "%d\n", available_slots);
}

void handle_eject(server_t *server, int client_index)
{
    player_t *player = get_player_by_fd(server,
                                        server->clients[client_index].fd);

    if (!player || player->is_dead) {
        dprintf(server->clients[client_index].fd, "dead\n");
        return;
    }
    dprintf(server->clients[client_index].fd, "ok\n");
}

