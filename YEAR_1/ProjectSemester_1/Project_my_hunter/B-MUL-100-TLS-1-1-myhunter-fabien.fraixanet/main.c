/*
** EPITECH PROJECT, 2023
** main
** File description:
** main function
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "include/graphic.h"

void free_mem(GLOB_T *all)
{
    sfTexture_destroy(all->picture[0].texture);
    sfSprite_destroy(all->picture[0].sprite);
    sfTexture_destroy(all->picture[1].texture);
    sfSprite_destroy(all->picture[1].sprite);
    sfRenderWindow_destroy(all->setting.window);
}

void click(GLOB_T *all)
{
    sfVector2i mouse = sfMouse_getPosition(all->setting.window);

    if (all->setting.event.type == sfEvtMouseButtonPressed &&
        (mouse.x >= 816 && mouse.x <= 648) &&
        (mouse.y >= 904 && mouse.y <= 949)
        ) {
    }
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
        if (all->setting.event.type == sfEvtKeyPressed
            && all->setting.event.key.code == sfKeySpace) {
        }
    }
}

void game(GLOB_T *all)
{
    while (sfRenderWindow_isOpen(all->setting.window)) {
        game_check_close(all);
        sfRenderWindow_clear(all->setting.window, sfBlack);
        sfRenderWindow_drawSprite(all->setting.window,
                                    all->picture[0].sprite, NULL);
        sfRenderWindow_drawSprite(all->setting.window,
                                    all->picture[1].sprite, NULL);
        sfRenderWindow_display(all->setting.window);
    }
    free_mem(all);
}

int main(void)
{
    GLOB_T all;
    sfMusic *music = NULL;
    int res = 0;

    res = init(&all);
    if (res == 84) {
        write(2, "An error occurred when setting up the application\n", 50);
        return (84);
    }
    start_music(music);
    game(&all);
    if (music != NULL) {
        sfMusic_destroy(music);
    }
    return (0);
}
