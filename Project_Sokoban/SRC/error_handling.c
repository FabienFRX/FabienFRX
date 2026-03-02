/*
** EPITECH PROJECT, 2023
** error_handling
** File description:
** function for error
*/

#include "../include/my.h"

int error_handling(char *map)
{
    int lenght = my_strlen(map);
    int count_P = 0;
    int count_X = 0;
    int count_O = 0;

    for (int i = 0; i <= lenght; i++) {
        if (map[i] == 'P') {
            count_P++;
        }
        if (map[i] == 'X') {
            count_X++;
        }
        if (map[i] == 'O') {
            count_O++;
        }
    }
    if (count_P != 1 || count_X != count_O) {
        return (84);
    }
    return (0);
}
