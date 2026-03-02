/*
** EPITECH PROJECT, 2023
** wincond
** File description:
** winning condition
*/

#include <ncurses.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../include/my.h"

static int find_target(char *map)
{
    int lenght = my_strlen(map);
    int res = 0;

    for (int i = 0; i <= lenght; i++) {
        if (map[i] == 'O') {
            res = res + 1;
        }
    }
    return (res);
}

static void exit_success(char *map)
{
    mvprintw(0, 0, map);
    endwin();
    free(map);
    exit(1);
}

int win_right(char *map, int idx)
{
    int targ = 0;

    map[idx + 2] = map[idx + 1];
    map[idx + 1] = map[idx];
    map[idx] = ' ';
    targ = find_target(map);
    if (targ == 0) {
        exit_success(map);
    }
    return (0);
}

int win_left(char *map, int idx)
{
    int targ = 0;

    map[idx - 2] = map[idx - 1];
    map[idx - 1] = map[idx];
    map[idx] = ' ';
    targ = find_target(map);
    if (targ == 0) {
        exit_success(map);
    }
    return (0);
}

int win_up(char *map, int idx, int line_lenght)
{
    int targ = 0;

    map[idx - (line_lenght * 2)] = map[idx - line_lenght];
    map[idx - line_lenght] = map[idx];
    map[idx] = ' ';
    targ = find_target(map);
    if (targ == 0) {
        exit_success(map);
    }
    return (0);
}

int win_down(char *map, int idx, int line_lenght)
{
    int targ = 0;

    map[idx + (line_lenght * 2)] = map[idx + line_lenght];
    map[idx + line_lenght] = map[idx];
    map[idx] = ' ';
    targ = find_target(map);
    if (targ == 0) {
        exit_success(map);
    }
    return (0);
}
