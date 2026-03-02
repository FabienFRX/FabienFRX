/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Basic command implementations
*/

#include "server.h"

void handle_inventory(server_t *server, int client_index)
{
    player_t *player = get_player_by_fd(server,
                                        server->clients[client_index].fd);

    if (!player || player->is_dead) {
        dprintf(server->clients[client_index].fd, "dead\n");
        return;
    }
    dprintf(server->clients[client_index].fd,
        "[food %d, linemate %d, deraumere %d, sibur %d, mendiane %d, phiras %d, thystame %d]\n",
        player->inventory[0], player->inventory[1], player->inventory[2],
        player->inventory[3], player->inventory[4], player->inventory[5], player->inventory[6]);
}

void handle_forward(server_t *server, int client_index)
{
    player_t *player = get_player_by_fd(server,
                                        server->clients[client_index].fd);

    if (!player || player->is_dead) {
        dprintf(server->clients[client_index].fd, "dead\n");
        return;
    }
    move_forward(player, server->map);
    dprintf(server->clients[client_index].fd, "ok\n");
    
    send_player_position(server, player);
}

void handle_left(server_t *server, int client_index)
{
    player_t *player = get_player_by_fd(server,
                                        server->clients[client_index].fd);

    if (!player || player->is_dead) {
        dprintf(server->clients[client_index].fd, "dead\n");
        return;
    }
    turn_left(player);
    dprintf(server->clients[client_index].fd, "ok\n");
    
    send_player_position(server, player);
}

void handle_right(server_t *server, int client_index)
{
    player_t *player = get_player_by_fd(server,
                                        server->clients[client_index].fd);

    if (!player || player->is_dead) {
        dprintf(server->clients[client_index].fd, "dead\n");
        return;
    }
    turn_right(player);
    dprintf(server->clients[client_index].fd, "ok\n");
    
    send_player_position(server, player);
}
