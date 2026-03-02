/*
** EPITECH PROJECT, 2023
** rush2 step1
** File description:
** count number of occurrences of letters
*/

#include <stdio.h>

void my_putchar(char c)
{
    write(1, &c, 1);
}

int my_put_nbr(int nb)
{
    if (nb > 9) {
        my_put_nbr(nb / 10);
    } else if (nb < 0) {
        nb *= (-1);
        write(1, '-', 1);
        my_put_nbr(nb / 10);
    }
    my_putchar(nb % 10 + '0');
    return 0;
}

int str_len(char const *str)
{
    int i = 0;

    while (str[i] != '\0') {
        i += 1;
    }
    return i;
}

int cmp_letters(char c1, char c2)
{
    if ((c1 > 64 && c1 < 91) && (c2 > 96 && c2 < 123)) {
            c2 = c2 - 32;
        }
    if ((c1 > 96 && c1 < 123) && (c2 > 64 && c2 < 91)) {
            c2 = c2 + 32;
        }
    if (c1 == c2) {
        return (1);
    }
    return (0);
}

int value(int a)
{
    int tmp = 0;

    if (tmp == a) {
    }
    while (str < '\0') {
        a = a + 1;
    }
}
