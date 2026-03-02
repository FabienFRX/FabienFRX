/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** GUI communication functions
*/

#include "server.h"

void send_to_gui_clients(server_t *server, const char *message)
{
    for (int i = 1; i <= server->client_count; i++) {
        if (server->clients[i].state == PROCESSING_COMMANDS && 
            server->clients[i].is_graphic) {
            dprintf(server->clients[i].fd, "%s\n", message);
        }
    }
}

void send_player_new(server_t *server, player_t *player)
{
    char message[256];
    snprintf(message, sizeof(message), "pnw #%d %d %d %d %d %s", 
             player->id, player->x, player->y, player->orientation, 
             player->level, get_player_team_name(server, player->id));
    send_to_gui_clients(server, message);
}

void send_player_position(server_t *server, player_t *player)
{
    char message[256];
    snprintf(message, sizeof(message), "ppo #%d %d %d %d", 
             player->id, player->x, player->y, player->orientation);
    send_to_gui_clients(server, message);
}

void send_player_level(server_t *server, player_t *player)
{
    char message[256];
    snprintf(message, sizeof(message), "plv #%d %d", 
             player->id, player->level);
    send_to_gui_clients(server, message);
}

void send_player_inventory(server_t *server, player_t *player)
{
    char message[256];
    snprintf(message, sizeof(message), "pin #%d %d %d %d %d %d %d %d %d %d", 
             player->id, player->x, player->y, 
             player->inventory[0], player->inventory[1], player->inventory[2],
             player->inventory[3], player->inventory[4], player->inventory[5],
             player->inventory[6]);
    send_to_gui_clients(server, message);
}

void send_player_death(server_t *server, player_t *player)
{
    char message[256];
    snprintf(message, sizeof(message), "pdi #%d", player->id);
    send_to_gui_clients(server, message);
}

void send_tile_content(server_t *server, int x, int y)
{
    char message[256];
    map_tile_t *tile = &server->map->tiles[y][x];
    snprintf(message, sizeof(message), "bct %d %d %d %d %d %d %d %d %d", 
             x, y, tile->resources[0], tile->resources[1], tile->resources[2],
             tile->resources[3], tile->resources[4], tile->resources[5],
             tile->resources[6]);
    send_to_gui_clients(server, message);
}

void send_map_size(server_t *server)
{
    char message[256];
    snprintf(message, sizeof(message), "msz %d %d", 
             server->map->width, server->map->height);
    send_to_gui_clients(server, message);
}

void send_all_tiles_content(server_t *server)
{
    for (int y = 0; y < server->map->height; y++) {
        for (int x = 0; x < server->map->width; x++) {
            send_tile_content(server, x, y);
        }
    }
} 