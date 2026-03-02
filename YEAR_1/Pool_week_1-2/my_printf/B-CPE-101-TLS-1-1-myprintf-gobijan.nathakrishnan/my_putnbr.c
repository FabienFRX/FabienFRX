/*
** EPITECH PROJECT, 2023
** goby
** File description:
** putnbrgoby
*/
#include <unistd.h>
#include "my.h"

void my_putnbr(int nb)
{
    if (nb == 0) {
        my_putchar('0');
    }
    if (nb < 0) {
        if (nb == -2147483648) {
            write(1, "-2147483648", 11);
        } else {
            nb = -nb;
            my_putchar('-');
            my_putchar(nb);
        }
    } else if (nb < 10) {
        my_putchar(nb + 48);
    } else {
        my_putnbr(nb / 10);
        my_putnbr(nb % 10);
    }
}
