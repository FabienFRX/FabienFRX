/*
** EPITECH PROJECT, 2023
** my_strdup
** File description:
** function qui alloue de la memoire et qui copie la string donné en parametre
*/
#include <stdlib.h>
#include "../../include/my.h"

char *my_strdup(char const *src)
{
    char *space;

    space = malloc(sizeof(char) * (my_strlen(src) + 1));
    if (space == NULL) {
        return (NULL);
    }
    my_strcpy(space, src);
    return (space);
}
