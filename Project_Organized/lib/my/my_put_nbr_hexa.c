/*
** EPITECH PROJECT, 2023
** my_put_nbr_hexa
** File description:
** display the number given in hexadecimally value
*/

#include "../../include/my.h"
#include <unistd.h>

int my_put_nbr_hexa(int nb)
{
    if (nb < 0) {
        nb = nb * (-1);
        my_putchar('-');
    }
    if (nb > 15) {
        my_put_nbr_hexa(nb / 16);
    }
    if (nb % 16 >= 10) {
        my_putchar(nb % 16 - 10 + 'a');
    } else {
        my_putchar(nb % 16 + '0');
    }
    return (0);
}
