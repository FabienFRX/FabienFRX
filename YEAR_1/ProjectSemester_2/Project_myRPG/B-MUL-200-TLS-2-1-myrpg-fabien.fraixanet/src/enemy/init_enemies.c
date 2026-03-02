/*
** EPITECH PROJECT, 2023
** B-MUL-200-TLS-2-1-myrpg-fabien.fraixanet
** File description:
** init_enemies.c
*/

#include "../../include/rpg.h"

static enemies_t *pushe(sfVector2f size, sfVector2f position,
    int id, sfSprite **sprite)
{
    enemies_t *tmp = malloc(sizeof(enemies_t));
    float ajuste = 1920.0 / 1080.0;
    sfSprite *tmpS = sprite[sprite_id("enemies")];

    sfSprite_setPosition(tmpS, position);
    if (tmp == NULL)
        return NULL;
    tmp->position = position;
    tmp->size = size;
    tmp->hitbox = create_rect(size, position, sfTransparent);
    tmp->sprite = tmpS;
    tmp->next = NULL;
    return tmp;
}

static sfVector2f rand_pos(sfIntRect spawn)
{
    sfVector2f position;

    position.x = (float)((rand() % (spawn.left - spawn.width)) + spawn.left);
    position.y = (float)((rand() % (spawn.top - spawn.height)) + spawn.top);
    return position;
}

sfIntRect spawn_id(int colli)
{
    switch (colli) {
        case 30:
        return (sfIntRect){319 * 5, 154 * 5, 580 * 5, 828 * 5};
        case 31:
        return (sfIntRect){13660, 3993, 15885, 4643};
        case 32:
        return (sfIntRect){13109, 558, 14832, 1474};
    }
}

enemies_t *init_enemies(sfIntRect spawn_area, sfSprite **sprites)
{
    int nb_enemies = (rand() % 6) + 4;
    sfVector2f pos = rand_pos(spawn_area);
    enemies_t *enemies = pushe((sfVector2f){50, 50}, pos, 0, sprites);
    start_t *s = malloc(sizeof(start_t));

    if (s == NULL)
        return NULL;
    s->first = enemies;
    for (int i = 1; i < nb_enemies; i++) {
        pos = rand_pos(spawn_area);
        enemies->next = pushe((sfVector2f){50, 50}, pos, i, sprites);
        enemies = enemies->next;
    }
    return s->first;
}
