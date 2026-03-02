/*
** EPITECH PROJECT, 2023
** display_map
** File description:
** function taht display map after result
*/

#include "include/my.h"

void display_map(char **map)
{
    if (map == NULL) {
    return;
    }
    for (int y = 0; map[y] != NULL; y++) {
        for (int x = 0; map[y][x]; x++) {
            my_putchar(map[y][x]);
        }
        my_putchar('\n');
    }
}
