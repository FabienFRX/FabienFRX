/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Player utility functions
*/

#include <math.h>
#include <stdlib.h>
#include "server.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int count_team_players(server_t *server, const char *team_name)
{
    int count = 0;
    int i = 0;

    for (i = 1; i <= server->client_count; i++) {
        if (server->clients[i].state == PROCESSING_COMMANDS &&
            !server->clients[i].is_graphic &&
            strcmp(server->clients[i].team_name, team_name) == 0) {
            count++;
        }
    }
    return count;
}

static void send_handshake_responses(server_t *server, int i,
                                     const char *team_name)
{
    dprintf(server->clients[i].fd, "%d\n", server->args->clients_nb);
    dprintf(server->clients[i].fd, "%d %d\n",
            server->args->width, server->args->height);
    server->clients[i].state = PROCESSING_COMMANDS;
    server->clients[i].is_graphic = false;
    strcpy(server->clients[i].team_name, team_name);
}

static void add_player_to_server(server_t *server, player_t *new_player, int i)
{
    server->players[server->player_count++] = new_player;
    server->clients[i].player_id = new_player->id;
}

void create_player_for_client(server_t *server, int i, const char *team_name)
{
    player_t *new_player = NULL;

    send_handshake_responses(server, i, team_name);
    new_player = create_player_from_egg(server, server->clients[i].fd,
                                        team_name);
    if (!new_player) {
        new_player = create_player_direct(server, server->clients[i].fd,
                                          team_name);
    }
    if (!new_player) {
        dprintf(server->clients[i].fd, "ko\n");
        remove_client(server, i);
        return;
    }
    add_player_to_server(server, new_player, i);
    
    send_player_new(server, new_player);
}

static int wrap_x(int dx, int width)
{
    double div_width = floor(width / 2.0);
    
    if (abs(dx) > div_width) {
        if (dx > 0) {
            dx -= width;
        } else {
            dx += width;
        }
    }
    return dx;
}

static int wrap_y(int dy, int height)
{
    double div_height = floor(height / 2.0);
    
    if (abs(dy) > div_height) {
        if (dy > 0) {
            dy -= height;
        } else {
            dy += height;
        }
    }
    return dy;
}

static double get_degrees(int src_x, int src_y, int recv_x, int recv_y, int width, int height)
{
    int dx = src_x - recv_x;
    int dy = src_y - recv_y;
    double angle_rad;
    double angle_degrees;
    
    dx = wrap_x(dx, width);
    dy = wrap_y(dy, height);
    
    angle_rad = -atan2(dy, dx);
    angle_degrees = fmod(angle_rad * 180.0 / M_PI, 360.0);
    if (angle_degrees < 0) angle_degrees += 360.0;
    
    return angle_degrees;
}

static int get_case(int direction, double angle_degrees)
{
    int start_degrees = 0;
    
    switch (direction) {
        case 1: start_degrees = 90; break;
        case 2: start_degrees = 0; break;
        case 3: start_degrees = 270; break;
        case 4: start_degrees = 180; break;
        default: start_degrees = 0; break;
    }
    
    double relative_angle = fmod(angle_degrees - start_degrees + 360.0, 360.0);
    
    if (relative_angle >= 337.5 || relative_angle < 22.5) return 1;
    if (relative_angle >= 22.5 && relative_angle < 67.5) return 2;
    if (relative_angle >= 67.5 && relative_angle < 112.5) return 3;
    if (relative_angle >= 112.5 && relative_angle < 157.5) return 4;
    if (relative_angle >= 157.5 && relative_angle < 202.5) return 5;
    if (relative_angle >= 202.5 && relative_angle < 247.5) return 6;
    if (relative_angle >= 247.5 && relative_angle < 292.5) return 7;
    if (relative_angle >= 292.5 && relative_angle < 337.5) return 8;
    
    return 0;
}

int calculate_direction(player_t *sender, player_t *receiver, map_t *map)
{
    if (!sender || !receiver || !map)
        return 0;
    
    if (sender->x == receiver->x && sender->y == receiver->y)
        return 0;
    
    double angle = get_degrees(sender->x, sender->y, receiver->x, receiver->y, 
                              map->width, map->height);
    
    return get_case(sender->orientation, angle);
}

void handle_broadcast(server_t *server, int client_index, const char *text)
{
    player_t *sender = get_player_by_fd(server,
                                        server->clients[client_index].fd);
    int i = 0;

    if (!sender || sender->is_dead) {
        dprintf(server->clients[client_index].fd, "dead\n");
        return;
    }
    for (i = 0; i < server->player_count; i++) {
        player_t *receiver = server->players[i];
        if (receiver && !receiver->is_dead && receiver->id != sender->id) {
            int direction = calculate_direction(sender, receiver, server->map);
            dprintf(receiver->id, "message %d, %s\n", direction, text);
        }
    }
    dprintf(server->clients[client_index].fd, "ok\n");
}

char *get_player_team_name(server_t *server, int player_id)
{
    for (int i = 1; i <= server->client_count; i++) {
        if (server->clients[i].fd == player_id)
            return server->clients[i].team_name;
    }
    return NULL;
}
