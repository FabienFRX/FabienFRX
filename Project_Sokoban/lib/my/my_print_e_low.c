/*
** EPITECH PROJECT, 2023
** my_print_e_low
** File description:
** flag e low
*/

#include <unistd.h>
#include "../../include/my.h"

float my_print_e(int *exponent, float tmp)
{
    if (tmp == 0) {
        write(1, "0.0e+00", 7);
    }
    if (tmp < 0) {
        tmp = -tmp;
        my_putchar('-');
    }
    while (tmp >= 10) {
        tmp /= 10;
        *exponent = *exponent + 1;
    }
    while (tmp < 1) {
        tmp *= 10;
        *exponent = *exponent - 1;
    }
    return (tmp);
}

void my_print_e_low(double f)
{
    int exponent = 0;
    int int_part = f;
    float tmp = f;

    tmp = my_print_e(&exponent, tmp);
    my_print_float(tmp, 6);
    my_putchar('e');
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
