/*
** EPITECH PROJECT, 2024
** B-MUL-100-TLS-1-1-myradar-fabien.fraixanet
** File description:
** init_pla
*/

#include <unistd.h>
#include <stdlib.h>
#include "include/graphic.h"
#include "include/struct.h"
#include "include/init.h"
#include "include/my.h"

int init_plane(GLOB_T *all)
{
    all->picture[3].texture = sfTexture_createFromFile("plane_min.png", NULL);
    if (all->picture[3].texture == NULL) {
        return (84);
    }
    all->picture[3].sprite = sfSprite_create();
    if (all->picture[3].sprite == NULL) {
        return (84);
    }
    all->picture[3].scale.x = scale_pla_x;
    all->picture[3].scale.y = scale_pla_y;
    all->picture[3].pos.x = plane_x;
    all->picture[3].pos.y = plane_y;
    return (0);
}

int plane_2nd(GLOB_T *all)
{
    all->picture[4].texture = sfTexture_createFromFile("plane_min.png", NULL);
    if (all->picture[4].texture == NULL) {
        return (84);
    }
    all->picture[4].sprite = sfSprite_create();
    if (all->picture[4].sprite == NULL) {
        return (84);
    }
    all->picture[4].scale.x = scale_pla2_x;
    all->picture[4].scale.y = scale_pla2_y;
    all->picture[4].pos.x = plane2_x;
    all->picture[4].pos.y = plane2_y;
    return (0);
}

int plane_3rd(GLOB_T *all)
{
    all->picture[5].texture = sfTexture_createFromFile("plane_min.png", NULL);
    if (all->picture[5].texture == NULL) {
        return (84);
    }
    all->picture[5].sprite = sfSprite_create();
    if (all->picture[5].sprite == NULL) {
        return (84);
    }
    all->picture[5].scale.x = scale_pla3_x;
    all->picture[5].scale.y = scale_pla3_y;
    all->picture[5].pos.x = plane3_x;
    all->picture[5].pos.y = plane3_y;
    return (0);
}

int init_sprite_pla(GLOB_T *all)
{
    if (all->picture == NULL)
        return (84);
    if (init_plane(all) == 84)
        return (84);
    if (plane_2nd(all) == 84)
        return (84);
    if (plane_3rd(all) == 84)
        return (84);
    sfSprite_setTexture(all->picture[3].sprite,
                        all->picture[3].texture, sfTrue);
    sfSprite_setScale(all->picture[3].sprite, all->picture[3].scale);
    sfSprite_setPosition(all->picture[3].sprite, all->picture[3].pos);
    sfSprite_setTexture(all->picture[4].sprite,
                        all->picture[4].texture, sfTrue);
    sfSprite_setScale(all->picture[4].sprite, all->picture[4].scale);
    sfSprite_setPosition(all->picture[4].sprite, all->picture[4].pos);
    sfSprite_setTexture(all->picture[5].sprite,
                        all->picture[5].texture, sfTrue);
    sfSprite_setScale(all->picture[5].sprite, all->picture[5].scale);
    sfSprite_setPosition(all->picture[5].sprite, all->picture[5].pos);
    return (0);
}

int inity(GLOB_T *all)
{
    if (init_sprite_pla(all) == 84) {
        return (84);
    }
    return (0);
}
