/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Time management functions
*/

#include "server.h"

void update_player_food(server_t *server, player_t *player)
{
    double real_time_ms = (126.0 / server->args->freq) * 1000.0;
    double current_time = get_current_time();
    double time_since_last_food = current_time - player->last_time;
    
    if (time_since_last_food >= real_time_ms) {
        if (player->inventory[0] > 0) {
            player->inventory[0]--;
        }
        if (player->inventory[0] <= 0) {
            player->inventory[0] = 0;
            player->is_dead = 1;
        }
        player->last_time = current_time;
    }
    
    double remaining_time = real_time_ms - time_since_last_food;
    if (remaining_time <= 0) {
        remaining_time = real_time_ms;
    }
    player->timeout = (int)remaining_time;
}

void check_all_players_food(server_t *server)
{
    static double last_food_check = 0;
    double current_time = get_current_time();
    
    if (current_time - last_food_check < 500.0) {
        return;
    }
    last_food_check = current_time;
    
    for (int i = 0; i < server->player_count; i++) {
        if (server->players[i] && !server->players[i]->is_dead) {
            update_player_food(server, server->players[i]);
        }
    }
}

int min_timeout(server_t *server)
{
    int timeout = 1000;
    int min_food = INT_MAX;
    bool found_player = false;

    for (int i = 0; i < server->player_count; i++) {
        if (server->players[i] && !server->players[i]->is_dead) {
            found_player = true;
            if (server->players[i]->timeout < min_food)
                min_food = server->players[i]->timeout;
        }
    }
    
    if (found_player && min_food < timeout)
        timeout = min_food;
    
    return timeout;
}

void process_timed_actions(server_t *server)
{
    double current_time = get_current_time();
    int i = 0;

    for (i = 0; i < server->player_count; i++) {
        if (!server->players[i] || server->players[i]->is_dead ||
            !server->players[i]->is_executing_action)
            continue;
        if (current_time >= server->players[i]->action_end_time) {
            execute_timed_command(server, server->players[i]);
        }
    }
}