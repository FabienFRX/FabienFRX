/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-sbmlparser-fabien.fraixanet
** File description:
** error_handling
*/

#include "../include/syn.h"

void my_putchar_error(char c)
{
    write(2, &c, 1);
}

int my_strlen(char const *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    return (i);
}

int my_putstr_error(const char *str)
{
    write(2, str, my_strlen(str));
    return (84);
}

int my_put_nbr_error(int nb)
{
    if (nb < 0) {
        nb = nb * (-1);
        my_putchar_error('-');
    }
    if (nb > 9) {
        my_put_nbr_error(nb / 10);
    }
    my_putchar_error(nb % 10 + '0');
    return (0);
}
