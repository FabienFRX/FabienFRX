/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Connection handling functions
*/

#include "server.h"

static void handle_gui_handshake(server_t *server, int i)
{
    server->clients[i].is_graphic = true;
    dprintf(server->clients[i].fd, "msz %d %d\n",
            server->args->width, server->args->height);
    server->clients[i].state = PROCESSING_COMMANDS;
}

static int check_team_exists(server_t *server, char *team_name)
{
    int j = 0;

    for (j = 0; server->args->team_names[j]; j++) {
        if (strcmp(server->args->team_names[j], team_name) == 0)
            return 1;
    }
    return 0;
}

static int count_active_players(server_t *server, const char *team_name)
{
    int active_players = 0;
    int j = 0;

    for (j = 1; j <= server->client_count; j++) {
        if (server->clients[j].state == PROCESSING_COMMANDS &&
            !server->clients[j].is_graphic &&
            strcmp(server->clients[j].team_name, team_name) == 0) {
            active_players++;
        }
    }
    return active_players;
}

static void handle_ai_handshake(server_t *server, int i, char *team_name)
{
    int active_players = 0;
    int available_eggs = 0;
    int total_allowed_players = 0;

    if (!check_team_exists(server, team_name)) {
        dprintf(server->clients[i].fd, "ko\n");
        remove_client(server, i);
        return;
    }
    active_players = count_active_players(server, team_name);
    available_eggs = count_team_eggs(server, team_name);
    total_allowed_players = server->args->clients_nb + available_eggs;
    if (active_players >= total_allowed_players) {
        dprintf(server->clients[i].fd, "ko\n");
        remove_client(server, i);
        return;
    }
    create_player_for_client(server, i, team_name);
}

void handle_handshake(server_t *server, int i, char *team_name)
{
    if (strcmp(team_name, "GRAPHIC") == 0) {
        handle_gui_handshake(server, i);
    } else {
        handle_ai_handshake(server, i, team_name);
    }
    printf("[SERVER] Après handshake, client %d state = %d (0=AWAITING_TEAM_NAME, 1=PROCESSING_COMMANDS)\n", i, server->clients[i].state);
}
