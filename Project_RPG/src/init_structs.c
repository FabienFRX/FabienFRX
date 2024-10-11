/*
** EPITECH PROJECT, 2023
** B-MUL-200-TLS-2-1-myrpg-fabien.fraixanet
** File description:
** init_structs.c
*/

#include "../include/rpg.h"

static sfRenderWindow *createwindow(int width, int height, const char *title)
{
    sfRenderWindow *Window;
    sfVideoMode mode;

    mode.width = width;
    mode.height = height;
    mode.bitsPerPixel = 32;
    Window = sfRenderWindow_create(mode, title, sfResize | sfClose, NULL);
    return (Window);
}

window_t *init_window(void)
{
    window_t *window = malloc(sizeof(window_t));
    sfTexture *texture =
        gen_background("./src_texture/ma/map.png");

    window->background = create_sprite(texture, (sfVector2f){0.0, 0.0});
    sfSprite_setScale(window->background, (sfVector2f){5.0 * 1.7777, 5.0});
    window->mode.bitsPerPixel = 32;
    window->mode.width = 1920.0;
    window->mode.height = 1080.0;
    window->size_w.x = window->mode.width;
    window->size_w.y = window->mode.height;
    window->title = "RPG";
    window->window = createwindow(window->size_w.x,
        window->size_w.y, window->title);
    window->view = create_view(&window->window,
        (sfVector2f){window->size_w.x / 2.0, window->size_w.y / 2.0});
    window->sprites = create_sprite_array();
    return window;
}
