/*
** EPITECH PROJECT, 2023
** main
** File description:
** main function
*/

#include <unistd.h>
#include <stdlib.h>
#include "include/graphic.h"
#include "include/struct.h"
#include "include/my.h"

void free_mem(GLOB_T *all)
{
    sfTexture_destroy(all->picture[0].texture);
    sfSprite_destroy(all->picture[0].sprite);
    sfTexture_destroy(all->picture[1].texture);
    sfSprite_destroy(all->picture[1].sprite);
    sfTexture_destroy(all->picture[2].texture);
    sfSprite_destroy(all->picture[2].sprite);
    sfTexture_destroy(all->picture[3].texture);
    sfSprite_destroy(all->picture[3].sprite);
    sfTexture_destroy(all->picture[4].texture);
    sfSprite_destroy(all->picture[4].sprite);
    sfTexture_destroy(all->picture[5].texture);
    sfSprite_destroy(all->picture[5].sprite);
    sfRenderWindow_destroy(all->setting.window);
}

void game_check_close(GLOB_T *all)
{
    while (sfRenderWindow_pollEvent(all->setting.window,
                    &all->setting.event)) {
        if (all->setting.event.type == sfEvtClosed) {
            sfRenderWindow_close(all->setting.window);
        }
        if (all->setting.event.type == sfEvtKeyPressed
            && all->setting.event.key.code == sfKeyEscape) {
            sfRenderWindow_close(all->setting.window);
        }
    }
}

void init_game(GLOB_T *all)
{
    while (sfRenderWindow_isOpen(all->setting.window)) {
        game_check_close(all);
        sfRenderWindow_clear(all->setting.window, sfBlack);
        sfRenderWindow_drawSprite(all->setting.window,
                                    all->picture[0].sprite, NULL);
        sfRenderWindow_drawSprite(all->setting.window,
                                    all->picture[1].sprite, NULL);
        sfRenderWindow_drawSprite(all->setting.window,
                                    all->picture[2].sprite, NULL);
        sfRenderWindow_drawSprite(all->setting.window,
                                    all->picture[3].sprite, NULL);
        sfRenderWindow_drawSprite(all->setting.window,
                                    all->picture[4].sprite, NULL);
        sfRenderWindow_drawSprite(all->setting.window,
                                    all->picture[5].sprite, NULL);
        sfRenderWindow_display(all->setting.window);
    }
    free_mem(all);
}

int main(int argc, char **argv)
{
    GLOB_T all;
    int res = 0;

    if (argc == 2 && my_strcmp(argv[1], "-h") == 0) {
        display_h();
        return (0);
    }
    res = init(&all);
    if (res == 84) {
        write(2, "An error occurred when setting up the application\n", 50);
        return (84);
    }
    init_game(&all);
    return (0);
}
