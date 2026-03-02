/*
** EPITECH PROJECT, 2023
** texture
** File description:
** texture_my_hunter
*/

#include <stdio.h>
#include <stdlib.h>
#include "include/graphic.h"

void init_game_win(GLOB_T *all)
{
    all->setting.mode.bitsPerPixel = 32;
    all->setting.mode.height = 1200;
    all->setting.mode.width = 675;
    all->setting.window = sfRenderWindow_create(all->setting.mode,
        "My_hunter_game", sfResize | sfClose, NULL);
}

int init_game_back(GLOB_T *all)
{
    sfTexture *texture_t = sfTexture_createFromFile("./land.jpg", NULL);
    sfSprite *texture_s = sfSprite_create();
    sfTexture *cursor_t = sfTexture_createFromFile("./sword_v.jpg", NULL);
    sfSprite *cursor_s = sfSprite_create();

    if (texture_t == NULL || cursor_t == NULL) {
        return (84);
    }
    if (texture_s == NULL || cursor_s == NULL) {
        return (84);
    }
    sfSprite_setTexture(texture_s, texture_t, sfTrue);
    sfSprite_setTexture(cursor_s, cursor_t, sfTrue);
    sfRenderWindow_drawSprite(all->setting.window, texture_s, NULL);
    sfRenderWindow_drawSprite(all->setting.window, cursor_s, NULL);
    sfSprite_destroy(cursor_s);
    sfTexture_destroy(cursor_t);
    sfSprite_destroy(texture_s);
    sfTexture_destroy(texture_t);
    return (0);
}
