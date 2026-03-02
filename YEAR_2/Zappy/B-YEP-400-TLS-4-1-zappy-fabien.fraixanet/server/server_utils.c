/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Server utility functions
*/

#include "server.h"

void error_exit(const char *msg)
{
    perror(msg);
    exit(84);
}

player_t *get_player_by_fd(server_t *server, int fd)
{
    int i = 0;

    for (i = 0; i < server->player_count; i++) {
        if (server->players[i] && server->players[i]->id == fd) {
            return server->players[i];
        }
    }
    return NULL;
}

void remove_client(server_t *server, int i)
{
    close(server->fds[i].fd);
    for (int j = i; j < server->client_count; j++) {
        server->fds[j] = server->fds[j + 1];
        server->clients[j] = server->clients[j + 1];
    }
    server->client_count--;
}

void print_string_details(const char *label __attribute__((unused)), const char *str __attribute__((unused)))
{
}
