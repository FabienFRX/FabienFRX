/*
** EPITECH PROJECT, 2024
** manage errors
** File description:
** qwerty
*/

#include "../include/amazed.h"

void my_putchar_error(const char c)
{
    write(2, &c, 1);
}

void my_putstr_error(const char *str)
{
    write(2, str, my_strlen(str));
}

void my_put_nbr_error(int nb)
{
    if (nb < 0) {
        my_putchar_error('-');
        nb = nb * -1;
    }
    if (nb >= 10) {
        my_put_nbr_error(nb / 10);
        my_put_nbr_error(nb % 10);
    } else {
        my_putchar_error(nb + '0');
    }
}
