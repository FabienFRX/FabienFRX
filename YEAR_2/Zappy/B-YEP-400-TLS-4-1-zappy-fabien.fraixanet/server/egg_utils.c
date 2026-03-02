/*
** EPITECH PROJECT, 2025
** B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
** File description:
** Egg utility functions
*/

#include "server.h"
#include <stdlib.h>

egg_t *create_egg_at_position(server_t *server, const char *team_name, int x, int y)
{
    egg_t *egg = malloc(sizeof(egg_t));
    if (!egg)
        return NULL;
    egg->id = server->next_egg_id++;
    egg->x = x;
    egg->y = y;
    strcpy(egg->team_name, team_name);
    egg->is_hatched = false;
    egg->creation_time = get_current_time();
    return egg;
}

egg_t *find_oldest_egg(server_t *server, const char *team_name)
{
    egg_t *oldest = NULL;
    double oldest_time = 0.0;
    for (int i = 0; i < server->egg_count; i++) {
        egg_t *egg = server->eggs[i];
        if (egg && !egg->is_hatched && strcmp(egg->team_name, team_name) == 0) {
            if (!oldest || egg->creation_time < oldest_time) {
                oldest = egg;
                oldest_time = egg->creation_time;
            }
        }
    }
    return oldest;
}
