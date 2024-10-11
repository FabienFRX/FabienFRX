/*
** EPITECH PROJECT, 2024
** B-MUL-200-TLS-2-1-myrpg-fabien.fraixanet
** File description:
** direction
*/

#include "../../include/rpg.h"


void moovedown(player_t *player, float frame_duration)
{
    sfTime tmp_time = {0};

    tmp_time = sfClock_getElapsedTime(player->clock);
    if (sfTime_asSeconds(tmp_time) >= frame_duration) {
        player->textureRect.left += 180;
        player->textureRect.top = 0;
        if (player->textureRect.left == 720)
            player->textureRect.left = 0;
        sfClock_restart(player->clock);
    player->elapsedTime = 0.001f;
    }
}

void mooveup(player_t *player, float frame_duration)
{
    sfTime tmp_time = {0};

    tmp_time = sfClock_getElapsedTime(player->clock);
    if (sfTime_asSeconds(tmp_time) >= frame_duration) {
        player->textureRect.left += 180;
        player->textureRect.top = 565;
        if (player->textureRect.left == 720)
            player->textureRect.left = 0;
        sfClock_restart(player->clock);
    player->elapsedTime = 0.001f;
    }
}

void mooveright(player_t *player, float frame_duration)
{
    sfTime tmp_time = {0};

    tmp_time = sfClock_getElapsedTime(player->clock);
    if (sfTime_asSeconds(tmp_time) >= frame_duration) {
        player->textureRect.left += 180;
        player->textureRect.top = 380;
        if (player->textureRect.left == 720)
            player->textureRect.left = 0;
        sfClock_restart(player->clock);
    player->elapsedTime = 0.001f;
    }
}

void mooveleft(player_t *player, float frame_duration)
{
    sfTime tmp_time = {0};

    tmp_time = sfClock_getElapsedTime(player->clock);
    if (sfTime_asSeconds(tmp_time) >= frame_duration) {
        player->textureRect.left += 180;
        player->textureRect.top = 195;
        if (player->textureRect.left == 720)
            player->textureRect.left = 0;
        sfClock_restart(player->clock);
    player->elapsedTime = 0.001f;
    }
}

void iddle(player_t *player, float frame_duration)
{
    sfTime tmp_time = {0};

    tmp_time = sfClock_getElapsedTime(player->clock);
    if(sfTime_asSeconds(tmp_time) >= (frame_duration * 2)) {
        player->textureRect.left += 180;
        player->textureRect.top = 750;
        if (player->textureRect.left == 720)
            player->textureRect.left = 0;
        sfClock_restart(player->clock);
    player->elapsedTime = 0.001f;
    }
}