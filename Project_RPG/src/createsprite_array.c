/*
** EPITECH PROJECT, 2023
** B-MUL-200-TLS-2-1-myrpg-fabien.fraixanet
** File description:
** createsprite_array.c
*/

#include "../include/rpg.h"

int sprite_id(char *mnémonique)
{
    for (int i = 0; sprite_tab[i].filename != NULL; i++) {
        if (!strcmp(sprite_tab[i].mnémonique, mnémonique)) {
            return i;
        }
    }
    return -1;
}

static size_t getsize_spritetab(void)
{
    size_t size = 0;

    while (sprite_tab[size].filename != NULL) {
        size++;
    }
    return size;
}

sfSprite **create_sprite_array(void)
{
    size_t size = getsize_spritetab();
    sfTexture *texture;
    sfSprite **sprites = malloc(size * sizeof(sfSprite *));

    for (size_t i = 0; i < size; ++i) {
        sprites[i] = sfSprite_create();
        texture = sfTexture_createFromFile(sprite_tab[i].filename, NULL);
        sfSprite_setTexture(sprites[i], texture, sfTrue);
        sfSprite_setOrigin(sprites[i], sprite_tab[i].origin);
        sfSprite_setPosition(sprites[i], sprite_tab[i].position);
        sfSprite_setScale(sprites[i], sprite_tab[i].scale);
    }
    return sprites;
}
