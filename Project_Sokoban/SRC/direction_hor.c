/*
** EPITECH PROJECT, 2023
** direction_hor
** File description:
** function that handle direction horizontal and set player position
*/

#include "../include/my.h"

int find_player(char *map)
{
    int lenght = my_strlen(map);

    for (int i = 0; i <= lenght; i++) {
        if (map[i] == 'P') {
            return (i);
        }
    }
    return (-1);
}

void direction_right(char *map)
{
    int idx = find_player(map);

    if (map[idx + 1] == ' ') {
        map[idx + 1] = 'P';
        map[idx] = ' ';
    }
    if (map[idx + 1] == 'X') {
        if (map[idx + 2] == 'O')
            win_right(map, idx);
        if (map[idx + 2] == ' ') {
            map[idx + 1] = 'P';
            map[idx + 2] = 'X';
            map[idx] = ' ';
        }
    }
}

void direction_left(char *map)
{
    int idx = find_player(map);

    if (map[idx - 1] == ' ') {
        map[idx - 1] = 'P';
        map[idx] = ' ';
    }
    if (map[idx - 1] == 'X') {
        if (map[idx - 2] == 'O')
            win_left(map, idx);
        if (map[idx - 2] == ' ') {
            map[idx - 1] = 'P';
            map[idx - 2] = 'X';
            map[idx] = ' ';
        }
    }
}
