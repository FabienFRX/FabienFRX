/*
** EPITECH PROJECT, 2024
** my_rpg
** File description:
** main my_rpg
*/

#include "../include/rpg.h"

int main(int argc, char **argv)
{
    window_t *window = init_window();
    player_t *player = create_player(player);
    sfEvent event;
    map_t *map = pars_node(map, window);
    bool pause = false;

    start(window, player, map, pause);
    sfRenderWindow_destroy(window->window);
    sfSprite_destroy(player->sprite);
    sfTexture_destroy(player->texture);
}
