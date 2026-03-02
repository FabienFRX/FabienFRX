/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Time management functions
*/

#include "server.h"
#include <sys/time.h>

double get_current_time(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1000000000.0;
}

void update_player_food(server_t *server, player_t *player)
{
    double current_time = get_current_time();
    double time_since_last_update = 0.0;
    double food_consumption_interval = 120.0 / server->args->freq;

    time_since_last_update = current_time - player->last_food_update;
    if (time_since_last_update >= food_consumption_interval) {
        if (player->inventory[0] > 0) {
            player->inventory[0]--;
            player->last_food_update = current_time;
            printf("[FOOD] Player %d consomme 1 nourriture, reste: %d\n", 
                   player->id, player->inventory[0]);
        }
        if (player->inventory[0] <= 0) {
            player->inventory[0] = 0;
            player->is_dead = 1;
            printf("[DEATH] Player %d est mort de faim!\n", player->id);
        }
    }
}

void check_all_players_food(server_t *server)
{
    static double last_check_time = 0;
    double current_time = get_current_time();
    double check_interval = 1.0;

    if (current_time - last_check_time < check_interval)
        return;
    last_check_time = current_time;
    
    for (int i = 0; i < server->player_count; i++) {
        if (server->players[i] && !server->players[i]->is_dead) {
            update_player_food(server, server->players[i]);
        }
    }
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
            printf("[TIMED_ACTIONS] Exécution de commande différée pour joueur %d\n", server->players[i]->id);
            execute_timed_command(server, server->players[i]);
        }
    }
}
