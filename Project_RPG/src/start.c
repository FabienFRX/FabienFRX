/*
** EPITECH PROJECT, 2023
** B-MUL-200-TLS-2-1-myrpg-fabien.fraixanet
** File description:
** start.c
*/

#include "../include/rpg.h"
//(sfVector2f){ * 5.0,  * 5.0};
static sfVector2f pos_by_id(int pnj)
{
    switch (pnj)
    {
        case 31:
        return (sfVector2f){1347.0 * 5.0, 29.0 * 5.0};
        case 32:
        return (sfVector2f){1252.0 * 5.0, 758.0 * 5.0};
        case 33:
        return (sfVector2f){524 * 5.0, 824 * 5.0};
    }
    return (sfVector2f){-1000.0, 0.0};
}

static map_t *add_items(map_t *map, int colli)
{
    start_t *s = malloc(sizeof(start_t));

    s->start = map;
    while (map->next != NULL) {
        map = map->next;
    }
    map->next = push((sfVector2f){44, 44}, pos_by_id(colli), colli - 20);
    return s->start;
}

static bool check_keypressed(int colli, player_t **player, map_t **map,
    sfSprite **sprites)
{
    if (colli != -1) {
        if (sfKeyboard_isKeyPressed(sfKeySpace) &&
            colli - 10 > 0 && colli - 10 < 4) {
            (*player)->inv = fillinv(colli - 10, (*player)->inv, map);
            return true;
        }
        if (sfKeyboard_isKeyPressed(sfKeySpace) &&
            colli <= 34 && colli > 30) {
                sleep(1);
                *map = add_items(*map, colli);
                //(*player)->enemies = init_enemies(spawn_id(colli), sprites);
        }
    }
    return false;
}

static void check_collision(map_t *map, window_t *window, player_t *player)
{
    map_t *tmp = map;
    int colli;

    for (int i = 0; tmp != NULL; i++) {
        colli = collision(tmp, player);
        sfRenderWindow_drawRectangleShape(window->window, tmp->walls, NULL);
        if (check_keypressed(colli, &player, &map, window->sprites))
            return;
        tmp = tmp->next;
    }
}

static void draw_enemies(enemies_t *enemies, window_t **window)
{
    enemies_t *tmp = enemies;

    while (tmp != NULL) {
        sfRenderWindow_drawSprite((*window)->window, tmp->sprite, NULL);
        tmp = tmp->next;
    }
}

void close_window(sfEvent event, sfRenderWindow *window)
{
    if (event.type == sfEvtClosed || sfKeyboard_isKeyPressed(sfKeyRControl)) {
            sfRenderWindow_close(window);
    } else {
        return;
    }
}

static void resizeview(window_t *window)
{
    float aspectRation = (float)sfRenderWindow_getSize(window->window).x /
        (float)sfRenderWindow_getSize(window->window).y;

    sfView_setSize(window->view, (sfVector2f){1920.0 * aspectRation, 1080.0});
}

static void check_event(sfEvent event, window_t *window,
    player_t *player, bool *pause)
{
    if (event.type == sfEvtResized) {
        resizeview(window);
    }
}

static void draw_pnj(window_t *window)
{
    for (int i = 0; sprite_tab[i].filename != NULL; i++) {
        if (strcmp(sprite_tab[i].mnémonique, "goldorack")) {
            sfRenderWindow_drawSprite(window->window, window->sprites[sprite_id("goldorack")], NULL);
        }
        if (strcmp(sprite_tab[i].mnémonique, "batman")) {
            sfRenderWindow_drawSprite(window->window, window->sprites[sprite_id("batman")], NULL);
        }
        if (strcmp(sprite_tab[i].mnémonique, "harlock")) {
            sfRenderWindow_drawSprite(window->window, window->sprites[sprite_id("harlock")], NULL);
        }
    }
}

void start(window_t *window, player_t *player, map_t *map, bool pause)
{
    sfEvent event;

    while (sfRenderWindow_isOpen(window->window)) {
        sfRenderWindow_clear(window->window, sfBlack);
        while (sfRenderWindow_pollEvent(window->window, &event)) {
            close_window(event, window->window);
            check_event(event, window, player, &pause);
        }
        sfRenderWindow_drawSprite(window->window, window->background, NULL);
        check_collision(map, window, player);
        inventory(window, window->sprites[sprite_id("inventaire")],
            player->inv);
        draw_pnj(window);
        if (pause == false) {
            update_player(player, window);
            check_move(player);
        }
        sfRenderWindow_display(window->window);
    }
}
