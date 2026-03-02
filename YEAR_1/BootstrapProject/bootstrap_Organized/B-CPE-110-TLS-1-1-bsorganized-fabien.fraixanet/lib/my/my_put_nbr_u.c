/*
** EPITECH PROJECT, 2023
** my_put_nbr_u
** File description:
** display the unsigned int given in parameters
*/
#include "../../include/my.h"

int my_put_nbr_u(int nb)
{
    long b;
    long sum = 4294967296;

    if (nb == 0) {
        my_putchar('0');
        return (0);
    }
    if (nb < 0) {
        b = (sum + nb);
        my_put_nbr(b);
        return b;
    }
    if (nb / 10)
        my_put_nbr_u(nb / 10);
    my_putchar(nb % 10 + '0');
    return (0);
}
