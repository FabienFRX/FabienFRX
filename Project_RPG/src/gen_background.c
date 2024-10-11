/*
** EPITECH PROJECT, 2023
** Untitled (Workspace)
** File description:
** gen_background.c
*/

#include "../include/rpg.h"

sfTexture *gen_background(char const *fp)
{
    sfTexture *texture = NULL;

    texture = sfTexture_createFromFile(fp, NULL);
    if (texture == NULL) {
        exit(EXIT_FAILURE);
    }
    return texture;
}
