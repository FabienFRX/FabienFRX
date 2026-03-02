/*
** EPITECH PROJECT, 2023
** my_put_nbr_base
** File description:
** display the number given at any base
*/
#include <unistd.h>
#include "../../include/my.h"

int my_put_nbr_base(int nb, int base)
{
    if (nb < 0) {
        nb = nb * (-1);
        my_putchar('-');
    }
    if (nb >= base) {
        my_put_nbr_base(nb / base, base);
    }
    if (nb % base >= 10) {
        my_putchar(nb % base - 10 + 'a');
    } else {
        my_putchar(nb % base + '0');
    }
    return (0);
}
