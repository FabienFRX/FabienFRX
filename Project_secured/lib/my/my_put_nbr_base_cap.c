/*
** EPITECH PROJECT, 2023
** my_put_nbr_base_cap
** File description:
** my_put_nbr_base by modified a low to a cap
*/

#include <unistd.h>
#include "../../include/my.h"

int my_put_nbr_base_cap(int nb, int base)
{
    if (nb < 0) {
        nb = nb * (-1);
        my_putchar('-');
    }
    if (nb >= base) {
        my_put_nbr_base_cap(nb / base, base);
    }
    if (nb % base >= 10) {
        my_putchar(nb % base - 10 + 'A');
    } else {
        my_putchar(nb % base + '0');
    }
    return (0);
}
