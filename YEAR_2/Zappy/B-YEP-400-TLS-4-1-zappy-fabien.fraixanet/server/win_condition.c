/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Win condition management functions
*/

#include "server.h"

char *check_win_condition(server_t *server)
{
    int team_idx = 0;
    int i = 0;
    int max_level_count = 0;

    if (!server->args->team_names)
        return NULL;
    for (team_idx = 0; server->args->team_names[team_idx]; team_idx++) {
        char *team_name = server->args->team_names[team_idx];
        max_level_count = 0;
        for (i = 0; i < server->player_count; i++) {
            char *player_team = get_player_team_name(server,
                                                     server->players[i]->id);
            if (server->players[i] && !server->players[i]->is_dead &&
                server->players[i]->level == 8 && player_team &&
                strcmp(player_team, team_name) == 0) {
                max_level_count++;
            }
        }
        if (max_level_count >= 6)
            return team_name;
    }
    return NULL;
}

void broadcast_win(server_t *server, const char *winning_team)
{
    int i = 0;

    for (i = 1; i <= server->client_count; i++) {
        if (server->clients[i].is_graphic) {
            dprintf(server->clients[i].fd, "seg %s\n", winning_team);
        } else {
            dprintf(server->clients[i].fd, "Game won by team %s\n",
                    winning_team);
        }
    }
}

void check_and_handle_win(server_t *server)
{
    char *winning_team = NULL;

    if (server->game_won)
        return;
    winning_team = check_win_condition(server);
    if (winning_team) {
        server->game_won = 1;
        broadcast_win(server, winning_team);
    }
}

void reset_game_state(server_t *server)
{
    int i = 0;

    for (i = 0; i < server->player_count; i++) {
        if (server->players[i])
            free(server->players[i]);
    }
    server->player_count = 0;
    cleanup_egg_system(server);
    init_egg_system(server);
    create_initial_eggs(server);
    spawn_initial_resources(server);
    server->game_won = 0;
}