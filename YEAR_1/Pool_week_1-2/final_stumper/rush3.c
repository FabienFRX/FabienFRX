/*
** EPITECH PROJECT, 2023
** oe
** File description:
** ooeoeo
*/

#include <unistd.h>
#include <stdlib.h>
#include "../../include/my.h"

int len_x(char *i, int j, int x)
{
    j = 0;
    while (i[j] != '\n') {
        x++;
        j++;
    }
    return x;
}

int len_y(char *i, int j, int y)
{
    j = 0;
    while (i[j] != '\0') {
        if (i[j] == '\n') {
            y++;
        }
        j++;
    }
    return y;
}

int rush1_1(char *i, int x, int y)
{
    if (i[0] == 'o') {
        write(1, "[rush1-1]", 9);
        my_putchar(' ');
        my_put_nbr(x);
        my_putchar(' ');
        my_put_nbr(y);
    }
    return (0);
}

int rush1_2(char *i, int x, int y)
{
    if (i[0] == '/' || i[0] == '*') {
        write(1, "[rush1-2]", 9);
        my_putchar(' ');
        my_put_nbr(x);
        my_putchar(' ');
        my_put_nbr(y);
    }
    return (0);
}

static int rush1_3(char *i, int x, int y)
{
    if (i[0] == 'A' && i[1] == 'B' && i[x] != 'C') {
        write(1, "[rush1-3]", 9);
        my_putchar(' ');
        my_put_nbr(x);
        my_putchar(' ');
        my_put_nbr(y);
    }
    return (0);
}

static int rush1_4(char *i, int x, int y)
{
    if (i[0] == 'A' && i[7] == 5) {
        write(1, "[rush1-4]", 9);
        my_putchar(' ');
        my_put_nbr(x);
        my_putchar(' ');
        my_put_nbr(y);
    }
    return (0);
}

static int rush1_5(char *i, int x, int y)
{
    if (i[0] == 'A' && i[7] == 5) {
        write(1, "[rush1-5]", 9);
        my_putchar(' ');
        my_put_nbr(x);
        my_putchar(' ');
        my_put_nbr(y);
    }
    return (0);
}

static int rush_test(char *i, int x, int y)
{
    if (i[0] == 'B') {
        write(1, "[rush1-3] ", 10);
        my_put_nbr(x);
        my_putchar(' ');
        my_put_nbr(y);
        write(1, " || [rush1-4] ", 14);
        my_put_nbr(x);
        my_putchar(' ');
        my_put_nbr(y);
        write(1, " || [rush1-5] ", 14);
        my_put_nbr(x);
        my_putchar(' ');
        my_put_nbr(y);
    }
    return (0);
}

int main(void)
{
    int x = 0;
    int y = 0;
    char i[32000];
    int j = 0;

    read(0, i, 31999);
    x = len_x(i, j, x);
    y = len_y(i, j, y);
    rush1_1(i, x, y);
    rush1_2(i, x, y);
    rush1_3(i, x, y);
    rush1_4(i, x, y);
    rush1_5(i, x, y);
    rush_test(i, x, y);
    my_putchar('\n');
    return (0);
}
