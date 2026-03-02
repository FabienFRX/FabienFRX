/*
** EPITECH PROJECT, 2023
** bootstrap_sokoban
** File description:
** my_popup
*/

#include <ncurses.h>
#include <stdlib.h>
#include "include/my.h"

int popup(int argc, char **argv)
{
    initscr();
    move(LINES / 2, (COLS / 2 - (my_strlen(argv[1]) / 2)));
    printw(argv[1]);
    refresh();
    getch();
    endwin();
    return (0);
}

int main(int argc, char **argv)
{
    if (argc == 1) {
        return (84);
    }
    popup(argc, argv);
    return (0);
}
