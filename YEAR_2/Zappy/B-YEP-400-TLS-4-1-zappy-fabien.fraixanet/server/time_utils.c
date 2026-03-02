/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Time utilities
*/

#include "server.h"
#include <sys/time.h>

double get_current_time(void)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000.0) + (time.tv_usec / 1000.0);
}

double get_difftime(double now, double last)
{
    double diff = (now - last) / 1000.0;
    return diff;
}

int convert_timeout(double time)
{
    int timeout = time * 1000;
    return timeout;
}

void init_time(player_t *player)
{
    player->now = get_current_time();
    player->last_time = player->now;
    player->timeout = 0;
}

double update_difftime(player_t *player)
{
    player->now = get_current_time();
    return get_difftime(player->now, player->last_time);
}

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