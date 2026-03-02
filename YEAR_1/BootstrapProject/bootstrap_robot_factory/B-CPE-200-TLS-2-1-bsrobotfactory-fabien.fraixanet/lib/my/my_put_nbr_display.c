/*
** EPITECH PROJECT, 2023
** my_put_nbr_display
** File description:
** my_put_nbr_display for rm '-'
*/

#include "../../include/my.h"
#include <unistd.h>

int my_put_nbr_display(int nb)
{
    if (nb < 0) {
        nb = nb * (-1);
    }
    if (nb > 9) {
        my_put_nbr(nb / 10);
    }
    my_putchar(nb % 10 + '0');
    return (0);
}
