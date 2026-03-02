/*
** EPITECH PROJECT, 2023
** put_nbr
** File description:
** displays the number given as parameters
*/
#include "../../include/my.h"
#include <unistd.h>

int my_put_nbr(int nb)
{
    if (nb < 0) {
        nb = nb * (-1);
        my_putchar('-');
    }
    if (nb > 9) {
        my_put_nbr(nb / 10);
    }
    my_putchar(nb % 10 + '0');
    return (0);
}
