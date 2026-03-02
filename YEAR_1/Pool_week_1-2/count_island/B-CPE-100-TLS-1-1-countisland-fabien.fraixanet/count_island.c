/*
** EPITECH PROJECT, 2023
** count_island
** File description:
** count island on a map
*/

#include <stddef.h>
#include "my.h"

int replace_x(char **world, int j, int i, int island)
{
    if (world[j][i] == 'X') {
        world[j][i] = island + '0';
        if (j - 1 >= 0)
            replace_x(world, j - 1, i, island);
        if (world[j + 1] != NULL)
            replace_x(world, j + 1, i, island);
        if (world[j][i + 1] != '\0')
            replace_x(world, j, i + 1, island);
        if (i - 1 >= 0)
            replace_x(world, j, i - 1, island);
        return (1);
    }
    return (0);
}

int count_island(char **world)
{
    int i = 0;
    int j = 0;
    int count = 0;

    while (world[j] != NULL) {
        while (world[j][i] != '\0') {
            count = count + replace_x(world, j, i, count);
            i = i + 1;
        }
        i = 0;
        j = j + 1;
    }
    return (count);
}
