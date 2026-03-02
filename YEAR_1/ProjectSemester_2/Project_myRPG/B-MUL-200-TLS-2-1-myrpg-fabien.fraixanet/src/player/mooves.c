/*
** EPITECH PROJECT, 2023
** B-MUL-200-TLS-2-1-myrpg-fabien.fraixanet
** File description:
** mooves.c
*/

#include "../../include/rpg.h"

static void check_movep2(player_t *player, float speed, float frame_duration)
{
    if (sfKeyboard_isKeyPressed(sfKeyLeft)) {
        sfSprite_move(player->sprite, (sfVector2f){(speed * -1.7777777), 0.0});
        mooveleft(player, frame_duration);
        return;
    }
    if (sfKeyboard_isKeyPressed(sfKeyRight)) {
        sfSprite_move(player->sprite, (sfVector2f){(speed * 1.7777777), 0.0});
        mooveright(player, frame_duration);
        return;
    }
    iddle(player, frame_duration);
    return;
}

void check_move(player_t *player)
{
    float speed = 0.3; /*0.3*/
    float frame_duration = player->frameDuration;

    if (sfKeyboard_isKeyPressed(sfKeyLControl)) {
        speed *= 2.0;
        frame_duration /= 2.0;
    }
    if (sfKeyboard_isKeyPressed(sfKeyUp)) {
        sfSprite_move(player->sprite, (sfVector2f){0.0, (speed * -1.0)});
        mooveup(player, frame_duration);
        return;
    }
    if (sfKeyboard_isKeyPressed(sfKeyDown)) {
        sfSprite_move(player->sprite, (sfVector2f){0.0, speed});
        moovedown(player, frame_duration);
        return;
    }
    check_movep2(player, speed, frame_duration);
}
