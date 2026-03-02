/*
** EPITECH PROJECT, 2023
** init
** File description:
** all initialisation
*/

#include <unistd.h>
#include <stdlib.h>
#include "include/graphic.h"
#include "include/struct.h"

void init_window(GLOB_T *all)
{
    all->setting.mode.bitsPerPixel = 32;
    all->setting.mode.height = HEIGHT;
    all->setting.mode.width = WIDTH;
    all->setting.window = sfRenderWindow_create(all->setting.mode,
        "My_hunter", sfResize | sfClose, NULL);
}

int init_title(GLOB_T *all)
{
    all->picture[0].texture = sfTexture_createFromFile("./land.jpg", NULL);
    if (all->picture[0].texture == NULL) {
        return (84);
    }
    all->picture[0].sprite = sfSprite_create();
    if (all->picture[0].sprite == NULL) {
        return (84);
    }
    all->picture[0].scale.x = 1;
    all->picture[0].scale.y = 1;
    sfSprite_setTexture(all->picture[0].sprite,
                        all->picture[0].texture, sfTrue);
    sfSprite_setScale(all->picture[0].sprite, all->picture[0].scale);
    return (0);
}

int init_duck(GLOB_T *all)
{
    all->picture[1].texture = sfTexture_createFromFile("./duck.png", NULL);
    if (all->picture[1].texture == NULL) {
        return (84);
    }
    all->picture[1].sprite = sfSprite_create();
    if (all->picture[1].sprite == NULL) {
        return (84);
    }
    all->picture[1].scale.x = SCALE_X;
    all->picture[1].scale.y = SCALE_Y;
    all->picture[1].pos.x = DUCK_X;
    all->picture[1].pos.y = DUCK_Y; 
    return (0);
}

int rect_duck(sfIntRect *DUCKT)
{
    DUCKT->top = 0;
    DUCKT->left = 0;
    DUCKT->width = 110;
    DUCKT->height = 110;
    return (84);
}
int init_sprite(GLOB_T *all)
{
    all->picture = malloc(sizeof(*all->picture) * 2);
    if (all->picture == NULL) {
        return (84);
    }
    if (init_title(all) == 84) {
        return (84);
    }
    if (init_duck(all) == 84) {
        return (84);
    }
    sfSprite_setTexture(all->picture[1].sprite,
                        all->picture[1].texture, sfTrue);
    sfSprite_setScale(all->picture[1].sprite, all->picture[1].scale);
    sfSprite_setPosition(all->picture[1].sprite, all->picture[1].pos);
    return (0);
}

int init(GLOB_T *all)
{
    init_window(all);
    if (all->setting.window == NULL) {
        return (84);
    }
    if (init_sprite(all) == 84) {
        return (84);
    }
    init_game_back(all);
    return (0);
}
