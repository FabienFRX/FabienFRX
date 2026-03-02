/*
** EPITECH PROJECT, 2023
** direction_ver
** File description:
** function that handle direction vertical
*/

#include "../include/my.h"

int find_line(char *map)
{
    int line_lenght = my_strlen(map);

    for (int i = 0; i <= line_lenght; i++) {
        if (map[i] == '\n') {
            return (i);
        }
    }
}

void direction_up(char *map)
{
    int line_lenght = find_line(map) + 1;
    int idx = find_player(map);
    int idx_up = idx - line_lenght;

    if (idx_up > 0 && map[idx_up] == ' ') {
        map[idx_up] = 'P';
        map[idx] = ' ';
    }
    if (map[idx_up] == 'X') {
        if (map[idx - (line_lenght * 2)] == 'O')
            win_up(map, idx, line_lenght);
        if (map[idx - (line_lenght * 2)] == ' ') {
            map[idx_up] = 'P';
            map[idx - (line_lenght * 2)] = 'X';
            map[idx] = ' ';
        }
    }
}

void direction_down(char *map)
{
    int line_lenght = find_line(map) + 1;
    int idx = find_player(map);
    int idx_down = idx + line_lenght;

    if (idx_down < my_strlen(map) && map[idx_down] == ' ') {
        map[idx_down] = 'P';
        map[idx] = ' ';
    }
    if (map[idx_down] == 'X') {
        if (map[idx + (line_lenght * 2)] == 'O')
            win_down(map, idx, line_lenght);
        if (map[idx + (line_lenght * 2)] == ' ') {
        map[idx_down] = 'P';
        map[idx + (line_lenght * 2)] = 'X';
        map[idx] = ' ';
        }
    }
}
