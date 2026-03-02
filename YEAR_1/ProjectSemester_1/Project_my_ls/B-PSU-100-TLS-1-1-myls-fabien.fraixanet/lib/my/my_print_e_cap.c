/*
** EPITECH PROJECT, 2023
** my_print_e_cap
** File description:
** flag e cap
*/

#include <unistd.h>
#include "../../include/my.h"

void my_print_e_cap(double f)
{
    int exponent = 0;
    int int_part = f;
    float tmp = f;

    tmp = my_print_e(&exponent, tmp);
    my_print_float(tmp, 6);
    my_putchar('E');
    if (int_part == 0) {
        my_putchar('-');
    } else {
        my_putchar('+');
    }
    if (exponent < 10 || exponent > -10) {
        my_putchar('0');
    }
    my_put_nbr_display(exponent);
}
