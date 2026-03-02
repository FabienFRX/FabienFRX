/*
** EPITECH PROJECT, 2023
** B-MUL-200-TLS-2-1-myrpg-fabien.fraixanet
** File description:
** collision.c
*/

#include "../../include/rpg.h"

static void collied(sfVector2f colli, float delta_x,
    float delta_y, player_t *p)
{
    if (colli.x > colli.y) {
        if (delta_x > 0.0) {
            sfSprite_move(p->sprite, (sfVector2f){colli.x * (-1.0), 0.0});
        } else {
            sfSprite_move(p->sprite, (sfVector2f){colli.x, 0.0});
        }
    } else {
        if (delta_y > 0.0) {
            sfSprite_move(p->sprite, (sfVector2f){0.0, colli.y * (-1.0)});
        } else {
            sfSprite_move(p->sprite, (sfVector2f){0.0, colli.y});
        }
    }
}

int collision(map_t *m, player_t *p)
{
    sfVector2f size_m = sfRectangleShape_getSize(m->walls);
    sfVector2f size_p = sfRectangleShape_getSize(p->hitbox);
    float delta_x = (p->h_position.x) - (m->position.x);
    float delta_y = (p->h_position.y) - (m->position.y);
    float colli_x = abs(delta_x) - ((size_m.x / 2.0) + (size_p.x / 2.0));
    float colli_y = abs(delta_y) - ((size_m.y / 2.0) + (size_p.y / 2.0));

    if (colli_x < 0.0 && colli_y < 0.0) {
        if (m->id == 0) {
            collied((sfVector2f){colli_x, colli_y}, delta_x, delta_y, p);
        }
        return m->id;
    }
    return -1;
}
