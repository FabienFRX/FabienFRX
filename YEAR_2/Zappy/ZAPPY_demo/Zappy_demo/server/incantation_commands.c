/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Incantation command implementation
*/

#include "server.h"

static void handle_level_one_incantation(server_t *server, int client_index,
                                         player_t *player)
{
    map_tile_t *tile = &server->map->tiles[player->y][player->x];

    if (tile->resources[1] >= 1) {
        tile->resources[1]--;
        dprintf(server->clients[client_index].fd, "Elevation underway\n");
        player->level = 2;
        dprintf(server->clients[client_index].fd, "Current level: 2\n");
    } else {
        dprintf(server->clients[client_index].fd, "ko\n");
    }
}

int count_level_players_on_tile(server_t *server, int x, int y,
                                       int level, const char *team)
{
    int count = 0;
    int i = 0;

    for (i = 0; i < server->player_count; i++) {
        player_t *other = server->players[i];
        char *other_team = NULL;

        if (!other || other->is_dead || other->level != level ||
            other->x != x || other->y != y)
            continue;
        other_team = get_player_team_name(server, other->id);
        if (other_team && strcmp(other_team, team) == 0)
            count++;
    }
    return count;
}

static void elevate_players_on_tile(server_t *server, int x, int y,
                                   int level, const char *team)
{
    int i = 0;
    int j = 0;

    for (i = 0; i < server->player_count; i++) {
        player_t *other = server->players[i];
        char *other_team = NULL;

        if (!other || other->is_dead || other->level != level ||
            other->x != x || other->y != y)
            continue;
        other_team = get_player_team_name(server, other->id);
        if (!other_team || strcmp(other_team, team) != 0)
            continue;
        for (j = 1; j <= server->client_count; j++) {
            if (server->clients[j].fd == other->id) {
                dprintf(server->clients[j].fd, "Elevation underway\n");
                other->level = 3;
                dprintf(server->clients[j].fd, "Current level: 3\n");
                break;
            }
        }
    }
}

int start_incantation(server_t *server, int client_index)
{
    player_t *player = get_player_by_fd(server,
                                        server->clients[client_index].fd);
    char *team_name = NULL;
    int players_count = 0;
    map_tile_t *tile = NULL;
    int i = 0;

    if (!player || player->is_dead || player->level >= 8) {
        dprintf(server->clients[client_index].fd, "ko\n");
        return 0;
    }

    team_name = get_player_team_name(server, player->id);
    if (!team_name) {
        dprintf(server->clients[client_index].fd, "ko\n");
        return 0;
    }

    int required_players = 1;
    if (player->level == 2) required_players = 2;
    else if (player->level == 3) required_players = 2;
    else if (player->level == 4) required_players = 4;
    else if (player->level == 5) required_players = 4;
    else if (player->level == 6) required_players = 6;
    else if (player->level == 7) required_players = 6;

    players_count = count_level_players_on_tile(server, player->x, player->y,
                                               player->level, team_name);
    
    if (players_count < required_players) {
        dprintf(server->clients[client_index].fd, "ko\n");
        return 0;
    }

    tile = &server->map->tiles[player->y][player->x];
    
    if (player->level == 1) {
        if (tile->resources[1] < 1) {
            dprintf(server->clients[client_index].fd, "ko\n");
            return 0;
        }
    } else if (player->level == 2) {
        if (tile->resources[1] < 1 || tile->resources[2] < 1 || tile->resources[3] < 1) {
            dprintf(server->clients[client_index].fd, "ko\n");
            return 0;
        }
    } else if (player->level == 3) {
        if (tile->resources[1] < 2 || tile->resources[3] < 1 || tile->resources[5] < 2) {
            dprintf(server->clients[client_index].fd, "ko\n");
            return 0;
        }
    } else if (player->level == 4) {
        if (tile->resources[1] < 1 || tile->resources[2] < 1 || tile->resources[3] < 2 || tile->resources[5] < 1) {
            dprintf(server->clients[client_index].fd, "ko\n");
            return 0;
        }
    } else if (player->level == 5) {
        if (tile->resources[1] < 1 || tile->resources[2] < 2 || tile->resources[3] < 1 || tile->resources[4] < 3) {
            dprintf(server->clients[client_index].fd, "ko\n");
            return 0;
        }
    } else if (player->level == 6) {
        if (tile->resources[1] < 1 || tile->resources[2] < 2 || tile->resources[3] < 3 || tile->resources[5] < 1) {
            dprintf(server->clients[client_index].fd, "ko\n");
            return 0;
        }
    } else if (player->level == 7) {
        if (tile->resources[1] < 2 || tile->resources[2] < 2 || tile->resources[3] < 2 || 
            tile->resources[4] < 2 || tile->resources[5] < 2 || tile->resources[6] < 1) {
            dprintf(server->clients[client_index].fd, "ko\n");
            return 0;
        }
    }

    for (i = 0; i < server->player_count; i++) {
        player_t *other = server->players[i];
        char *other_team = NULL;

        if (!other || other->is_dead || other->level != player->level ||
            other->x != player->x || other->y != player->y)
            continue;
        other_team = get_player_team_name(server, other->id);
        if (!other_team || strcmp(other_team, team_name) != 0)
            continue;
        other->in_incantation = 1;
        for (int j = 1; j <= server->client_count; j++) {
            if (server->clients[j].fd == other->id) {
                dprintf(server->clients[j].fd, "Elevation underway\n");
                break;
            }
        }
    }
    return 1;
}

void end_incantation(server_t *server, int client_index)
{
    player_t *player = get_player_by_fd(server,
                                        server->clients[client_index].fd);
    char *team_name = NULL;
    int players_count = 0;
    map_tile_t *tile = NULL;
    int i = 0;

    if (!player || player->is_dead || !player->in_incantation) {
        dprintf(server->clients[client_index].fd, "ko\n");
        return;
    }

    team_name = get_player_team_name(server, player->id);
    if (!team_name) {
        dprintf(server->clients[client_index].fd, "ko\n");
        return;
    }

    int required_players = 1;
    if (player->level == 2) required_players = 2;
    else if (player->level == 3) required_players = 2;
    else if (player->level == 4) required_players = 4;
    else if (player->level == 5) required_players = 4;
    else if (player->level == 6) required_players = 6;
    else if (player->level == 7) required_players = 6;

    players_count = count_level_players_on_tile(server, player->x, player->y,
                                               player->level, team_name);
    
    if (players_count < required_players) {
        for (i = 0; i < server->player_count; i++) {
            player_t *other = server->players[i];
            char *other_team = NULL;
            if (!other || other->is_dead || other->level != player->level ||
                other->x != player->x || other->y != player->y)
                continue;
            other_team = get_player_team_name(server, other->id);
            if (!other_team || strcmp(other_team, team_name) != 0)
                continue;
            other->in_incantation = 0;
            for (int j = 1; j <= server->client_count; j++) {
                if (server->clients[j].fd == other->id) {
                    dprintf(server->clients[j].fd, "ko\n");
                    break;
                }
            }
        }
        return;
    }

    tile = &server->map->tiles[player->y][player->x];
    int fail = 0;
    if (player->level == 1) {
        if (tile->resources[1] < 1) fail = 1;
    } else if (player->level == 2) {
        if (tile->resources[1] < 1 || tile->resources[2] < 1 || tile->resources[3] < 1) fail = 1;
    } else if (player->level == 3) {
        if (tile->resources[1] < 2 || tile->resources[3] < 1 || tile->resources[5] < 2) fail = 1;
    } else if (player->level == 4) {
        if (tile->resources[1] < 1 || tile->resources[2] < 1 || tile->resources[3] < 2 || tile->resources[5] < 1) fail = 1;
    } else if (player->level == 5) {
        if (tile->resources[1] < 1 || tile->resources[2] < 2 || tile->resources[3] < 1 || tile->resources[4] < 3) fail = 1;
    } else if (player->level == 6) {
        if (tile->resources[1] < 1 || tile->resources[2] < 2 || tile->resources[3] < 3 || tile->resources[5] < 1) fail = 1;
    } else if (player->level == 7) {
        if (tile->resources[1] < 2 || tile->resources[2] < 2 || tile->resources[3] < 2 || 
            tile->resources[4] < 2 || tile->resources[5] < 2 || tile->resources[6] < 1) fail = 1;
    }
    if (fail) {
        for (i = 0; i < server->player_count; i++) {
            player_t *other = server->players[i];
            char *other_team = NULL;
            if (!other || other->is_dead || other->level != player->level ||
                other->x != player->x || other->y != player->y)
                continue;
            other_team = get_player_team_name(server, other->id);
            if (!other_team || strcmp(other_team, team_name) != 0)
                continue;
            other->in_incantation = 0;
            for (int j = 1; j <= server->client_count; j++) {
                if (server->clients[j].fd == other->id) {
                    dprintf(server->clients[j].fd, "ko\n");
                    break;
                }
            }
        }
        return;
    }
    if (player->level == 1) tile->resources[1]--;
    else if (player->level == 2) { tile->resources[1]--; tile->resources[2]--; tile->resources[3]--; }
    else if (player->level == 3) { tile->resources[1] -= 2; tile->resources[3]--; tile->resources[5] -= 2; }
    else if (player->level == 4) { tile->resources[1]--; tile->resources[2]--; tile->resources[3] -= 2; tile->resources[5]--; }
    else if (player->level == 5) { tile->resources[1]--; tile->resources[2] -= 2; tile->resources[3]--; tile->resources[4] -= 3; }
    else if (player->level == 6) { tile->resources[1]--; tile->resources[2] -= 2; tile->resources[3] -= 3; tile->resources[5]--; }
    else if (player->level == 7) { tile->resources[1] -= 2; tile->resources[2] -= 2; tile->resources[3] -= 2; tile->resources[4] -= 2; tile->resources[5] -= 2; tile->resources[6]--; }

    for (i = 0; i < server->player_count; i++) {
        player_t *other = server->players[i];
        char *other_team = NULL;
        if (!other || other->is_dead || other->level != player->level ||
            other->x != player->x || other->y != player->y)
            continue;
        other_team = get_player_team_name(server, other->id);
        if (!other_team || strcmp(other_team, team_name) != 0)
            continue;
        for (int j = 1; j <= server->client_count; j++) {
            if (server->clients[j].fd == other->id) {
                other->level = player->level + 1;
                dprintf(server->clients[j].fd, "Current level: %d\n", other->level);
                other->in_incantation = 0;
                break;
            }
        }
    }
}

void handle_incantation(server_t *server, int client_index)
{
    player_t *player = get_player_by_fd(server,
                                        server->clients[client_index].fd);
    
    if (!player || player->is_dead) {
        dprintf(server->clients[client_index].fd, "ko\n");
        return;
    }
    
    start_incantation(server, client_index);
}
