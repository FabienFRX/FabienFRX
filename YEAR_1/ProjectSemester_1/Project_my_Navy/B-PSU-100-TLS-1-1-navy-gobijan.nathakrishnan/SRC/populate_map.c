/*
** EPITECH PROJECT, 2024
** Populate_map
** File description:
** azerty
*/

#include "../include/my_navy.h"

void display_map(char **map, char **map_e)
{
    printf("my positions:\n");
    for (int i = 0; i < 10; i++) {
        printf("%s\n", map[i]);
    } 
    printf("\nenemy's positions:\n");
    for (int i = 0; i < 10; i++) {
        printf("%s\n", map_e[i]);
    }
    printf("\n");
}

static char **fill_first_line(char **map)
{
    char letter = 'A';
    
    for (int i = 0; i < 17; i += 2) {
        map[0][i] = ' ';
    }
    map[0][1] = '|';
    for (int i = 2; i < 17; i += 2) {
        map[0][i] = letter++;
    }
    map[0][17] = '\0';
    return map;
}

static char **fill_nbr(char **map)
{
    char number = '1';

    for (int i = 2; i < 10; i++, number++) {
        map[i][0] = number;
    }
    return map;
}

char **populate_empty_map(char **map)
{
    map = malloc(sizeof(char *) * 10);
    for (int i = 0; i < 10; i++) {
        map[i] = malloc(sizeof(char) * 17);
    }
    fill_first_line(map);
    fill_nbr(map);
    for (int i = 2; i < 10; i++) {
        map[i][1] = '|';
    }
    for (int i = 0; i < 17; i++) {
        map[1][i] = '-';
        if (i == 1) {
            map[1][i] = '+';
        }
    }
    for (int i = 2; i < 10; i++) {
        for (int j = 2; j < 17; j += 2) {
            map[i][j] = '.';
            map[i][j + 1] = ' ';
        }
    }
    return map;
}
