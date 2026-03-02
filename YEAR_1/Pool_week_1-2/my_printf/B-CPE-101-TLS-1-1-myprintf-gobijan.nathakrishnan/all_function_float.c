/*
** EPITECH PROJECT, 2023
** PRINT_FLOAT
** File description:
** trynna print a fuckin float numba
*/
#include <stdio.h>
#include <unistd.h>
#include "my.h"
#include "abs.h"

void my_putchar(char c)
{
    write(1, &c, 1);
}

void my_putnbr(int nb)
{
    if (nb < 0) {
        if (-2147483648 == nb) {
            write(1, "-2147483648", 11);
        } else {
            nb = -nb;
            my_putchar('-');
            my_putnbr(nb);
        }
    } else if (nb < 10) {
        my_putchar(nb + 48);
    } else {
        my_putnbr(nb / 10);
        my_putnbr(nb % 10);
    }
}

void my_print_float(double f, int decimals)
{
    int int_part = (int)f;
    if (f < 0) {
        f = -f;
    }
    my_putnbr(int_part);
    if (decimals != 0) {
    my_putchar('.');
    }
    double fract_part = f - ABS(int_part);
    for (int i = 0; i < decimals; i++) {
        fract_part *= 10;
        int digit = (int)fract_part;
        my_putnbr(digit);
        fract_part -= digit;
    }
}

int main(void)
{
    double positive = 12345.050505;
    double negative = -12345.050505;
    int d = 6;
    int no_d = 0;

    printf("%f\n", positive);
    my_print_float(positive, d);
    my_putchar('\n');
    my_print_float(negative, d);
    my_putchar('\n');
    my_print_float(positive, no_d);
    my_putchar('\n');
    my_print_float(negative, no_d);
    return 0;
}
