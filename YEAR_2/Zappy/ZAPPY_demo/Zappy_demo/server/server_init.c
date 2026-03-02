/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Server initialization functions
*/

#include "server.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

map_t *create_map(int width, int height)
{
    map_t *map = malloc(sizeof(map_t));
    if (!map)
        return NULL;
    
    map->width = width;
    map->height = height;
    map->tiles = malloc(height * sizeof(map_tile_t *));
    if (!map->tiles) {
        free(map);
        return NULL;
    }
    
    for (int y = 0; y < height; y++) {
        map->tiles[y] = malloc(width * sizeof(map_tile_t));
        if (!map->tiles[y]) {
            for (int i = 0; i < y; i++)
                free(map->tiles[i]);
            free(map->tiles);
            free(map);
            return NULL;
        }
        for (int x = 0; x < width; x++) {
            for (int r = 0; r < 7; r++) {
                map->tiles[y][x].resources[r] = 0;
            }
        }
    }
    
    return map;
}

static int init_server_socket(int port)
{
    int server_fd;
    struct sockaddr_in server_addr;
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket creation failed");
        return -1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        close(server_fd);
        return -1;
    }

    printf("Server listening on port %d\n", port);
    return server_fd;
}

void init_server(server_t *server, arguments_t *args)
{
    memset(server, 0, sizeof(server_t));
    server->args = args;
    server->port = args->port;
    server->map = create_map(args->width, args->height);
    server->players = malloc(sizeof(player_t *) * MAX_CLIENTS);
    server->player_count = 0;
    server->eggs = malloc(sizeof(egg_t *) * MAX_CLIENTS);
    server->egg_count = 0;
    server->next_egg_id = 1;
    server->game_won = 0;
    server->win_time = 0.0;
    server->evolution_site_count = 0;
    server->evolution_request_count = 0;
    
    server->server_fd = init_server_socket(args->port);
    if (server->server_fd < 0) {
        error_exit("Failed to initialize server socket");
    }
    
    server->fds[0].fd = server->server_fd;
    server->fds[0].events = POLLIN;
    server->client_count = 0;
}

void init_server_components(server_t *server)
{
    init_egg_system(server);
    create_initial_eggs(server);
    spawn_initial_resources(server);
    server->last_resource_spawn = get_current_time();
}
