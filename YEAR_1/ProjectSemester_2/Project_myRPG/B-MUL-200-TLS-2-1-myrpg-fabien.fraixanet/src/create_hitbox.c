/*
** EPITECH PROJECT, 2023
** B-MUL-200-TLS-2-1-myrpg-fabien.fraixanet
** File description:
** create_hitbox.c
*/

#include "../include/rpg.h"

sfRectangleShape *create_rectnorigin(sfVector2f size,
    sfVector2f position, sfColor color)
{
    sfRectangleShape *hitbox = sfRectangleShape_create();

    sfRectangleShape_setFillColor(hitbox, color);
    sfRectangleShape_setSize(hitbox, size);
    sfRectangleShape_setPosition(hitbox, position);
    return hitbox;
}


sfRectangleShape *create_rect(sfVector2f size,
    sfVector2f position, sfColor color)
{
    sfRectangleShape *hitbox = sfRectangleShape_create();

    sfRectangleShape_setFillColor(hitbox, color);
    sfRectangleShape_setSize(hitbox, size);
    sfRectangleShape_setOrigin(hitbox, (sfVector2f){size.x / 2, size.y / 2});
    sfRectangleShape_setPosition(hitbox, position);
    return hitbox;
}

sfCircleShape *create_circle(float radius, sfVector2f position, sfColor color)
{
    sfCircleShape *hitbox = sfCircleShape_create();

    sfCircleShape_setFillColor(hitbox, color);
    sfCircleShape_setRadius(hitbox, radius);
    sfCircleShape_setOrigin(hitbox, (sfVector2f){radius, radius});
    sfCircleShape_setPosition(hitbox, position);
    return hitbox;
}
