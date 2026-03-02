/*
** EPITECH PROJECT, 2024
** B-CPE-200-TLS-2-1-robotfactory-fabien.fraixanet [WSL: Debian]
** File description:
** error_management
*/

#include "../include/asm.h"

void my_putchar_error(char c)
{
    write(2, &c, 1);
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
