/*
** EPITECH PROJECT, 2023
** my_str_to_word_array
** File description:
** func that convert a string into char **
*/

#include "include/my.h"

int count_line(char *line)
{
    int count = 0;

    for (int x = 0; line[x] != '\0'; x++) {
        if (line[x] == '\n') {
            count++;
        }
    }
    return (count);
}

int count_line_size(char *line)
{
    int x = 0;

    for (; line[x] != '\n' && line[x] != '\0'; x++);
    return (x);
}

char **my_str_to_word_array(char *str)
{
    int x = count_line_size(str);
    int y = count_line(str);
    char **map = NULL;
    int str_idx = 0;

    map = malloc(sizeof(char *) * (y + 1));
    if (map == NULL) {
        return (NULL);
    }
    map[y] = NULL;
    for (int i = 0; i < y; i++) {
        map[i] = malloc(sizeof(char) * (x + 1));
        for (int j = 0; str[str_idx] != '\n'; j++) {
            map[i][j] = str[str_idx];
            str_idx++;
        }
        str_idx++;
        map[i][x] = '\0';
    }
    return (map);
}
