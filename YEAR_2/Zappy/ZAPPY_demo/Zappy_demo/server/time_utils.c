/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Time utility functions
*/

#include "server.h"
#include <string.h>
#include <time.h>

void init_command_time(player_t *player, double duration)
{
    double current_time = get_current_time();
    player->action_start_time = current_time;
    player->action_end_time = current_time + duration;
}

int is_command_finished(player_t *player)
{
    double current_time = get_current_time();
    return current_time >= player->action_end_time;
}

double get_remaining_time(player_t *player)
{
    double current_time = get_current_time();
    double remaining = player->action_end_time - current_time;
    return (remaining > 0) ? remaining : 0;
}

void execute_finished_command(server_t *server, player_t *player)
{
    int client_index = player->pending_client_index;
    const char *cmd = player->pending_command;
    if (strcmp(cmd, "Inventory") == 0) {
        handle_inventory(server, client_index);
    } else if (strcmp(cmd, "Look") == 0) {
        handle_look(server, client_index);
    } else if (strcmp(cmd, "Forward") == 0) {
        handle_forward(server, client_index);
    } else if (strcmp(cmd, "Right") == 0) {
        handle_right(server, client_index);
    } else if (strcmp(cmd, "Left") == 0) {
        handle_left(server, client_index);
    } else if (strncmp(cmd, "Take ", 5) == 0) {
        handle_take(server, client_index, cmd);
    } else if (strncmp(cmd, "Set ", 4) == 0) {
        handle_set(server, client_index, cmd);
    } else if (strcmp(cmd, "Incantation") == 0) {
        handle_incantation(server, client_index);
    } else if (strcmp(cmd, "Fork") == 0) {
        handle_fork(server, client_index);
    } else if (strncmp(cmd, "Broadcast ", 10) == 0) {
        handle_broadcast(server, client_index, cmd + 10);
    } else if (strcmp(cmd, "Eject") == 0) {
        handle_eject(server, client_index);
    }
} 