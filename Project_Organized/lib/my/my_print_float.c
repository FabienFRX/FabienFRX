/*
** EPITECH PROJECT, 2023
** my_print_float
** File description:
** function for flag f
*/
#include <unistd.h>
#include <stdio.h>
#include "../../include/my.h"
#include "../../include/abs.h"

void my_print_float(double f, int decimals)
{
    float fract_part;
    int digit;
    int int_part = f;

    if (f < 0) {
        f = -f;
    }
    my_put_nbr(int_part);
    my_putchar('.');
    fract_part = f - ABS(int_part);
    for (int i = 0; i < decimals; i++) {
        fract_part *= 10;
        digit = fract_part;
        my_put_nbr(digit);
        fract_part -= digit;
    }
}
