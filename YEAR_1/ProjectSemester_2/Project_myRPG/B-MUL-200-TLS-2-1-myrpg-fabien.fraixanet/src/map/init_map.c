/*
** EPITECH PROJECT, 2023
** B-MUL-200-TLS-2-1-myrpg-fabien.fraixanet
** File description:
** init_map : circular link list (last point to first)
*/

#include "../../include/rpg.h"

map_t *push(sfVector2f size, sfVector2f position, int id)
{
    map_t *tmp = malloc(sizeof(map_t));
    float ajuste = 1920.0 / 1080.0;

    position.x += (size.x / 2.0);
    position.y += (size.y / 2.0);
    if (tmp == NULL) {
        return NULL;
    }
    size.x *= ajuste;
    position.x *= ajuste;
    if (id == 0) {
        tmp->walls = create_rect(size, position, sfTransparent);
    } else if (id >= 10 && id <= 14) {
        tmp->walls = create_rect(size, position, sfYellow);
    } else {
        tmp->walls = create_rect(size, position, sfTransparent);
    }
    tmp->position = position;
    tmp->size = size;
    tmp->id = id;
    tmp->next = NULL;
    return tmp;
}

bool is_null(walls_t *walls)
{
    if (walls->position.x == 0 && walls->position.y == 0 &&
        walls->size.x == 0 && walls->size.y == 0) {
        return true;
    } else {
        return false;
    }
}

map_t *pars_node(map_t *map, window_t *wind)
{
    map_t *tmp;
    start_t *s = malloc(sizeof(start_t));

    if (s == NULL) {
        return NULL;
    }
    tmp = push(walls_tab[0].size, walls_tab[0].position,
        walls_tab[0].id);
    s->start = NULL;
    s->start = tmp;
    for (int i = 1; is_null(&walls_tab[i]) != true; i++) {
        tmp->next = push(walls_tab[i].size, walls_tab[i].position,
            walls_tab[i].id);
        tmp->next->prev = tmp;
        tmp = tmp->next;
    }
    return s->start;
}
