/*
** EPITECH PROJECT, 2023
** B-MUL-200-TLS-2-1-myrpg-fabien.fraixanet
** File description:
** init_inventory.c
*/

#include "../../include/rpg.h"

object_t obj_tab[] = {
    {1, "./src_texture/sprites/red_pot.png"},
    {2, "./src_texture/sprites/items.png"},
    {3, "./src_texture/sprites/mana.png"},
    {4, "./src_texture/sprites/rechargeable.png"},
    {0, "./src_texture/sprites/transparent.png"}
};

int *init_inventory(void)
{
    int *inv = malloc(sizeof(int) * 28);

    inv[27] = -1;
    for (int j = 0; inv[j] != -1; j++) {
        inv[j] = 0;
    }
    return inv;
}

static sfSprite *sprite_createfromfile(char *filename, int i)
{
    sfSprite *object = sfSprite_create();
    sfTexture *Texture_object = sfTexture_createFromFile(filename, NULL);
    sfVector2f position = {((((float)(i % 9)) * 36) * 6.0 * 1.77777) + 10020,
        ((i / 9) * 36) * 6.0 + 10000};

    sfSprite_setTexture(object, Texture_object, true);
    sfSprite_setPosition(object, position);
    sfSprite_setScale(object, (sfVector2f){6.0 * 1.77777, 6.0});
    return object;
}

static char *findfilenamefromid(int id)
{
    for (int i = 0; obj_tab[i].id_obj != 0; i++) {
        if (obj_tab[i].id_obj == id) {
            return obj_tab[i].filename;
        }
    }
    return NULL;
}

window_t *window_drawspritefromid(int *inv, window_t *window)
{
    for (int i = 0; inv[i] != -1; i++) {
        if (inv[i] != 0) {
            sfRenderWindow_drawSprite(window->window,
                sprite_createfromfile(findfilenamefromid(inv[i]), i), NULL);
        }
    }
    return window;
}
