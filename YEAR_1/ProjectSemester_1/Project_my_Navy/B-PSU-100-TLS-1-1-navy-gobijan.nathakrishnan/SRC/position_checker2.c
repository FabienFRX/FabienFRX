/*
** EPITECH PROJECT, 2024
** Position checker2
** File description:
** qwerty
*/

#include "../include/my_navy.h"

int char_to_int1(char c)
{
    return (c >= '1' && c <= '8') ? c - '0' : 0;
}

int char_to_int2(char c)
{
    return (c >= 'A' && c <= 'H') ? 2 * (c - 'A') + 2 : 0;
}

char **fill_col(char **map, char **pos, int line, char nb)
{
    int col = char_to_int2(pos[line][2]);

    for (int j = char_to_int1(pos[line][3]) + 1;
    j <= char_to_int1(pos[line][6]) + 1; j++) {
        map[j][col] = nb;
    }
    return map;
}

char **fill_line(char **map, char **pos, int line, char nb)
{
    int row = char_to_int1(pos[line][3]) + 1;
    int col = char_to_int2(pos[line][2]);
    int dif = pos[line][5] - pos[line][2] + 1;

    for (int j = 0; j < dif; col += 2, j++) {
        map[row][col] = nb;
    }
    return map;
}

char **fill_map_w_boats(char **map, char **pos)
{
    for (int i = 2; i <= 16; i++) {
        if (map[0][i] == pos[0][2] || map[0][i] == pos[0][5]) {
            map[char_to_int1(pos[0][3]) + 1][i] = '2';
            map[char_to_int1(pos[0][6]) + 1][i] = '2';
        }
    }
    for (int i = 1; i <= 3; i++) {
        if (map == NULL) {
            break;
        }
        if (pos[i][2] == pos[i][5]) {
            map = fill_col(map, pos, i, pos[i][0]);
        } else {
            map = fill_line(map, pos, i, pos[i][0]);
        }
    }
    return map;
}
