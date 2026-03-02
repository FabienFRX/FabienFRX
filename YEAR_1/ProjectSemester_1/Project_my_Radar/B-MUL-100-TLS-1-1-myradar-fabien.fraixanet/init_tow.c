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
#include "include/init.h"
#include "include/my.h"

void init_window(GLOB_T *all)
{
    all->setting.mode.bitsPerPixel = 32;
    all->setting.mode.height = HEIGHT;
    all->setting.mode.width = WIDTH;
    all->setting.window = sfRenderWindow_create(all->setting.mode,
        "my_radar", sfResize | sfClose, NULL);
}

static int init_map(GLOB_T *all)
{
    all->picture[0].texture = sfTexture_createFromFile("world_map.jpg", NULL);
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

int init_tower(GLOB_T *all)
{
    all->picture[1].texture = sfTexture_createFromFile("tower_min.png", NULL);
    if (all->picture[1].texture == NULL) {
        return (84);
    }
    all->picture[1].sprite = sfSprite_create();
    if (all->picture[1].sprite == NULL) {
        return (84);
    }
    all->picture[1].scale.x = scale_tow_x;
    all->picture[1].scale.y = scale_tow_y;
    all->picture[1].pos.x = tower_x;
    all->picture[1].pos.y = tower_y;
    return (0);
}

int tower_2nd(GLOB_T *all)
{
    all->picture[2].texture = sfTexture_createFromFile("tower_min.png", NULL);
    if (all->picture[2].texture == NULL) {
        return (84);
    }
    all->picture[2].sprite = sfSprite_create();
    if (all->picture[2].sprite == NULL) {
        return (84);
    }
    all->picture[2].scale.x = scale_tow2_x;
    all->picture[2].scale.y = scale_tow2_y;
    all->picture[2].pos.x = tower2_x;
    all->picture[2].pos.y = tower2_y;
    return (0);
}

int init_sprite(GLOB_T *all)
{
    all->picture = malloc(sizeof(*all->picture) * 6);
    if (all->picture == NULL)
        return (84);
    if (init_map(all) == 84)
        return (84);
    if (init_tower(all) == 84)
        return (84);
    if (tower_2nd(all) == 84)
        return (84);
    sfSprite_setTexture(all->picture[1].sprite,
                        all->picture[1].texture, sfTrue);
    sfSprite_setScale(all->picture[1].sprite, all->picture[1].scale);
    sfSprite_setPosition(all->picture[1].sprite, all->picture[1].pos);
    sfSprite_setTexture(all->picture[2].sprite,
                        all->picture[2].texture, sfTrue);
    sfSprite_setScale(all->picture[2].sprite, all->picture[2].scale);
    sfSprite_setPosition(all->picture[2].sprite, all->picture[2].pos);
    init_sprite_pla(all);
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
    return (0);
}
