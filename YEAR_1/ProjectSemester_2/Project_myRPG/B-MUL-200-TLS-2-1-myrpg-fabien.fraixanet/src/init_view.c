/*
** EPITECH PROJECT, 2023
** B-MUL-200-TLS-2-1-myrpg-fabien.fraixanet
** File description:
** init_view.c
*/

#include "../include/rpg.h"

sfView *create_view(sfRenderWindow **window, sfVector2f player_pos)
{
    sfView *view = sfView_create();

    update_view(window, player_pos, view);
    return view;
}

sfView *update_view(sfRenderWindow **window, sfVector2f player_pos,
    sfView *view)
{
    sfView_setCenter(view, player_pos);
    sfRenderWindow_setView(*window, view);
    return view;
}
