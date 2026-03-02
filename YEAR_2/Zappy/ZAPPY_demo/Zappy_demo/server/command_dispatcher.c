/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Command dispatching functions
*/

#include "server.h"
#include <sys/time.h>

void handle_client_command(server_t *server, int client_index, const char *cmd)
{
    player_t *player = get_player_by_fd(server, server->clients[client_index].fd);

    if (!player || player->is_dead) {
        dprintf(server->clients[client_index].fd, "dead\n");
        return;
    }

    if (player->is_executing_action) {
        return;
    }

    if (strcmp(cmd, "Inventory") == 0) {
        init_command_time(player, 1.0 / server->args->freq);
        strcpy(player->pending_command, cmd);
        player->pending_client_index = client_index;
        player->is_executing_action = 1;
        return;
    }
    if (strcmp(cmd, "Look") == 0) {
        init_command_time(player, 7.0 / server->args->freq);
        strcpy(player->pending_command, cmd);
        player->pending_client_index = client_index;
        player->is_executing_action = 1;
        return;
    }
    if (strcmp(cmd, "Forward") == 0 || strcmp(cmd, "Right") == 0 || strcmp(cmd, "Left") == 0) {
        init_command_time(player, 7.0 / server->args->freq);
        strcpy(player->pending_command, cmd);
        player->pending_client_index = client_index;
        player->is_executing_action = 1;
        return;
    }
    if (strncmp(cmd, "Take ", 5) == 0 || strncmp(cmd, "Set ", 4) == 0) {
        init_command_time(player, 7.0 / server->args->freq);
        strcpy(player->pending_command, cmd);
        player->pending_client_index = client_index;
        player->is_executing_action = 1;
        return;
    }
    if (strncmp(cmd, "Broadcast ", 10) == 0) {
        init_command_time(player, 7.0 / server->args->freq);
        strcpy(player->pending_command, cmd);
        player->pending_client_index = client_index;
        player->is_executing_action = 1;
        return;
    }
    if (strcmp(cmd, "Incantation") == 0) {
        if (start_incantation(server, client_index)) {
            init_command_time(player, 300.0 / server->args->freq);
            strcpy(player->pending_command, cmd);
            player->pending_client_index = client_index;
            player->is_executing_action = 1;
        }
        return;
    }
    if (strcmp(cmd, "Fork") == 0) {
        init_command_time(player, 42.0 / server->args->freq);
        strcpy(player->pending_command, cmd);
        player->pending_client_index = client_index;
        player->is_executing_action = 1;
        return;
    }
    if (strcmp(cmd, "Eject") == 0) {
        init_command_time(player, 7.0 / server->args->freq);
        strcpy(player->pending_command, cmd);
        player->pending_client_index = client_index;
        player->is_executing_action = 1;
        return;
    }
    dprintf(server->clients[client_index].fd, "ko\n");
}
