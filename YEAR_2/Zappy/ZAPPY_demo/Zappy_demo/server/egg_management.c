/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Egg management functions
*/

#include "server.h"

void init_egg_system(server_t *server)
{
    server->eggs = NULL;
    server->egg_count = 0;
    server->next_egg_id = 1;
}

void cleanup_egg_system(server_t *server)
{
    int i = 0;

    if (server->eggs) {
        for (i = 0; i < server->egg_count; i++) {
            if (server->eggs[i])
                free(server->eggs[i]);
        }
        free(server->eggs);
        server->eggs = NULL;
        server->egg_count = 0;
    }
}

static egg_t *create_single_egg(server_t *server, const char *team_name)
{
    egg_t *new_egg = malloc(sizeof(egg_t));

    if (!new_egg)
        return NULL;
    new_egg->id = server->next_egg_id++;
    new_egg->x = rand() % server->args->width;
    new_egg->y = rand() % server->args->height;
    strcpy(new_egg->team_name, team_name);
    new_egg->creation_time = get_current_time();
    new_egg->is_hatched = false;
    return new_egg;
}

void create_initial_eggs(server_t *server)
{
    int team_idx = 0;
    int i = 0;
    egg_t *new_egg = NULL;

    for (team_idx = 0; server->args->team_names[team_idx]; team_idx++) {
        for (i = 0; i < server->args->clients_nb; i++) {
            new_egg = create_single_egg(server,
                                        server->args->team_names[team_idx]);
            if (!new_egg)
                continue;
            add_egg_to_server(server, new_egg);
        }
    }
}

int count_team_eggs(server_t *server, const char *team_name)
{
    int count = 0;
    int i = 0;

    for (i = 0; i < server->egg_count; i++) {
        if (server->eggs[i] && !server->eggs[i]->is_hatched &&
            strcmp(server->eggs[i]->team_name, team_name) == 0) {
            count++;
        }
    }
    return count;
}

void add_egg_to_server(server_t *server, egg_t *egg)
{
    if (!egg)
        return;
    
    egg_t **new_eggs = realloc(server->eggs, 
                                (server->egg_count + 1) * sizeof(egg_t *));
    if (!new_eggs)
        return;
    
    server->eggs = new_eggs;
    server->eggs[server->egg_count++] = egg;
}
