/*
** EPITECH PROJECT, 2023
** replace_square
** File description:
** function that replace '.' by 'x'
*/

#include "include/my.h"

void inf_condition(char **map, coordonates_t *current,
    int *biggest_size, int origin_y)
{
    if (map[current->y] != NULL && map[current->y][current->x] != '.') {
        current->y = origin_y;
        *biggest_size = *biggest_size - 1;
    }
}

void check_square(char **map, int y, int x, coordonates_t *max)
{
    int biggest_size = 0;
    coordonates_t current = {y, x, 0};

    for (int i = x; map[y][i] == '.' && map[y][i] != '\0'; i++) {
        biggest_size++;
    }
    for (; current.y < (biggest_size + y) && biggest_size > max->size
        && map[current.y] != NULL; current.y++) {
        for (current.x = x; current.x < (biggest_size + x); current.x++) {
            inf_condition(map, &current, &biggest_size, y);
        }
    }
    if (biggest_size > max->size) {
        max->y = y;
        max->x = x;
        max->size = biggest_size;
    }
}

char **change_char(char **map, coordonates_t *max)
{
    for (int i = max->y; i < (max->size + max->y) && map[i] != NULL; i++) {
        for (int j = max->x; j < (max->size + max->x); j++) {
            map[i][j] = 'x';
        }
    }
    return (map);
}

int replace_square(char **map)
{
    coordonates_t max = {
        0,
        0,
        0,
    };

    for (int y = 0; map[y] != NULL; y++) {
        for (int x = 0; map[y][x] != '\0'; x++) {
            check_square(map, y, x, &max);
        }
    }
    map = change_char(map, &max);
    display_map(map);
    return (0);
}
