/*
** EPITECH PROJECT, 2023
** B-MUL-200-TLS-2-1-myrpg-fabien.fraixanet
** File description:
** create_sprite.c
*/

#include "../include/rpg.h"

sfSprite *create_sprite(sfTexture *texture, sfVector2f position)
{
    sfSprite *sprite = sfSprite_create();
    sfVector2u scale;
    sfVector2f center;

    sfSprite_setTexture(sprite, texture, sfTrue);
    sfSprite_setPosition(sprite, position);
    return sprite;
}
