/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Resource management functions
*/

#include "server.h"

static void clear_all_tiles(server_t *server)
{
    int x = 0;
    int y = 0;
    int r = 0;

    for (y = 0; y < server->map->height; y++) {
        for (x = 0; x < server->map->width; x++) {
            for (r = 0; r < 7; r++) {
                server->map->tiles[y][x].resources[r] = 0;
            }
        }
    }
}

static void spawn_resource_type(server_t *server, int resource, int quantity)
{
    int i = 0;
    int x = 0;
    int y = 0;

    for (i = 0; i < quantity; i++) {
        x = rand() % server->map->width;
        y = rand() % server->map->height;
        server->map->tiles[y][x].resources[resource]++;
    }
}

void spawn_initial_resources(server_t *server)
{
    const double densities[7] = {0.3, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05};
    const int minimums[7] = {15, 15, 7, 5, 5, 4, 3};
    int total_tiles = server->map->width * server->map->height;
    int resource = 0;
    int quantity = 0;

    clear_all_tiles(server);
    for (resource = 0; resource < 7; resource++) {
        quantity = (int)(total_tiles * densities[resource]);
        if (quantity < minimums[resource])
            quantity = minimums[resource];
        spawn_resource_type(server, resource, quantity);
    }
}

void spawn_periodic_resources(server_t *server)
{
    double current_time = get_current_time();
    double time_unit = 20.0 / server->args->freq;
    const double periodic_densities[7] = {0.005, 0.005, 0.003, 0.002,
                                          0.002, 0.001, 0.001};
    int total_tiles = server->map->width * server->map->height;
    int resource = 0;

    if (current_time - server->last_resource_spawn < time_unit)
        return;
    for (resource = 0; resource < 7; resource++) {
        int quantity = (int)(total_tiles * periodic_densities[resource]);
        if (quantity > 0)
            spawn_resource_type(server, resource, quantity);
    }
    server->last_resource_spawn = current_time;
}
