/*
** EPITECH PROJECT, 2023
** main
** File description:
** main my_sokoban
*/

#include <ncurses.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../include/my.h"

int size_of(char const *file)
{
    struct stat all_info;

    if (stat(file, &all_info) == -1){
        return (-84);
    }
    return all_info.st_size;
}

char *getmap(char *filename)
{
    int fd = open(filename, O_RDONLY);
    int lenght = size_of(filename);
    char *map;
    long int reading;

    if (fd == -1) {
        return (NULL);
    }
    map = malloc(sizeof(char) * (lenght + 1));
    if (map == NULL) {
        return (NULL);
    }
    reading = read(fd, map, lenght);
    close(fd);
    if (reading == -1) {
        return (NULL);
    }
    map[lenght] = '\0';
    return (map);
}

int check_direction(char *map, int ch)
{
    switch (ch) {
    case KEY_RIGHT:
        direction_right(map);
        break;
    case KEY_LEFT:
        direction_left(map);
        break;
    case KEY_UP:
        direction_up(map);
        break;
    case KEY_DOWN:
        direction_down(map);
        break;
    default:
        break;
    }
    return (0);
}

int box_create(char *map)
{
    int ch = 0;

    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    mvprintw(0, 0, map);
    noecho();
    curs_set(0);
    while (ch != 27 && ch != 'q') {
        ch = getch();
        check_direction(map, ch);
        mvprintw(0, 0, map);
    }
    endwin();
    return (0);
}

int main(int argc, char **argv)
{
    char *map = NULL;

    if (argc == 2 && my_strcmp(argv[1], "-h") == 0) {
        disp_help();
        return (0);
    }
    if (argc != 2) {
        return (84);
    }
    map = getmap(argv[1]);
    if (map == NULL) {
        return (84);
    }
    if (error_handling(map) == 84) {
        return (84);
    }
    box_create(map);
    return (0);
}
