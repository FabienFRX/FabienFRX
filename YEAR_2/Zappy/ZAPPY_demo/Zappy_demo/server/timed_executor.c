/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Timed command execution functions
*/

#include "server.h"

int is_immediate_command(const char *cmd)
{
    if (strcmp(cmd, "Inventory") == 0 || strcmp(cmd, "Look") == 0 ||
        strcmp(cmd, "Connect_nbr") == 0 || strncmp(cmd, "Take ", 5) == 0 ||
        strncmp(cmd, "Set ", 4) == 0 || strcmp(cmd, "Fork") == 0)
        return 1;
    return 0;
}

static void execute_movement_command(server_t *server, player_t *player,
                                     const char *cmd)
{
    int client_index = player->pending_client_index;

    if (strcmp(cmd, "Forward") == 0)
        handle_forward(server, client_index);
    else if (strcmp(cmd, "Right") == 0)
        handle_right(server, client_index);
    else if (strcmp(cmd, "Left") == 0)
        handle_left(server, client_index);
}

static void execute_special_command(server_t *server, player_t *player,
                                    const char *cmd)
{
    int client_index = player->pending_client_index;

    if (strncmp(cmd, "Broadcast ", 10) == 0) {
        const char *text = cmd + 10;
        handle_broadcast(server, client_index, text);
    } else if (strcmp(cmd, "Eject") == 0) {
        handle_eject(server, client_index);
    }
}

void execute_timed_command(server_t *server, player_t *player)
{
    int client_index = 0;
    
    for (int i = 1; i <= server->client_count; i++) {
        if (server->clients[i].fd == player->id) {
            client_index = i;
            break;
        }
    }
    
    if (client_index == 0) {
        printf("[TIMED_EXECUTOR] Erreur: client_index non trouvé pour joueur %d\n", player->id);
        return;
    }
    
    printf("[TIMED_EXECUTOR] Exécution de la commande: %s\n", player->pending_command);
    
    if (strcmp(player->pending_command, "Inventory") == 0) {
        handle_inventory(server, client_index);
    } else if (strcmp(player->pending_command, "Look") == 0) {
        handle_look(server, client_index);
    } else if (strncmp(player->pending_command, "Take ", 5) == 0) {
        handle_take(server, client_index, player->pending_command);
    } else if (strncmp(player->pending_command, "Set ", 4) == 0) {
        handle_set(server, client_index, player->pending_command);
    } else if (strcmp(player->pending_command, "Forward") == 0) {
        handle_forward(server, client_index);
    } else if (strcmp(player->pending_command, "Right") == 0) {
        handle_right(server, client_index);
    } else if (strcmp(player->pending_command, "Left") == 0) {
        handle_left(server, client_index);
    } else if (strncmp(player->pending_command, "Broadcast ", 10) == 0) {
        handle_broadcast(server, client_index, player->pending_command + 10);
    } else if (strcmp(player->pending_command, "Eject") == 0) {
        handle_eject(server, client_index);
    } else if (strcmp(player->pending_command, "Incantation") == 0) {
        printf("[TIMED_EXECUTOR] Exécution de la phase 2 de l'incantation\n");
        end_incantation(server, client_index);
    } else {
        printf("[TIMED_EXECUTOR] Commande inconnue: %s\n", player->pending_command);
    }
    
    player->is_executing_action = 0;
    memset(player->pending_command, 0, sizeof(player->pending_command));
    player->pending_client_index = 0;
}

void handle_fork(server_t *server, int client_index)
{
    player_t *player = get_player_by_fd(server,
                                        server->clients[client_index].fd);
    char *team_name = server->clients[client_index].team_name;
    egg_t *new_egg = NULL;

    if (!player || player->is_dead) {
        dprintf(server->clients[client_index].fd, "dead\n");
        return;
    }
    new_egg = create_egg_at_position(server, team_name,
                                     player->x, player->y);
    if (!new_egg) {
        dprintf(server->clients[client_index].fd, "ko\n");
        return;
    }
    add_egg_to_server(server, new_egg);
    dprintf(server->clients[client_index].fd, "ok\n");
}
