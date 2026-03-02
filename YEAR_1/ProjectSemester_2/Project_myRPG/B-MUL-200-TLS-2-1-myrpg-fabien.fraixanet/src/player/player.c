/*
** EPITECH PROJECT, 2023
** B-MUL-200-TLS-2-1-myrpg-fabien.fraixanet
** File description:
** player.c
*/

#include "../../include/rpg.h"

player_t *create_player(player_t *player)
{
    player = malloc(sizeof(player_t));
    player->name = "Player187392710";
    player->position = (sfVector2f){3000.0, 1000.0};
    player->textureRect = (sfIntRect){0, 0, 180, 200};
    player->elapsedTime = 0.0005f;
    player->frameDuration = 0.15f;
    player->clock = sfClock_create();
    player->texture = sfTexture_createFromFile("./src_texture/hero2test.png",
        NULL);
    player->sprite = create_sprite(player->texture, player->position);
    sfSprite_setScale(player->sprite, (sfVector2f){1.5, 1});
    player->h_size = (sfVector2f){100.0, 75.0};
    player->h_position = (sfVector2f){(player->position.x + player->h_size.x) / 2.0,
        (player->position.y + player->h_size.y) / 2.0};
    player->hitbox = create_rect((sfVector2f){100.0, 75.0},
        player->h_position, sfTransparent);
    player->inv = init_inventory();
    player->enemies = NULL;
    return player;
}

static void update_hitbox(player_t *player)
{
    player->h_position = (sfVector2f){player->position.x + 140.0,
        player->position.y + 150.0};
    sfRectangleShape_setPosition(player->hitbox, player->h_position);
}

void update_player(player_t *player, window_t *window)
{
    player->position = sfSprite_getPosition(player->sprite);
    sfSprite_setPosition(player->sprite, player->position);
    window->view = update_view(&window->window,
        player->position, window->view);
    update_hitbox(player);
    sfRenderWindow_drawSprite(window->window, player->sprite, NULL);
    sfSprite_setTextureRect(player->sprite, player->textureRect);
    sfRenderWindow_drawRectangleShape(window->window, player->hitbox, NULL);
}
