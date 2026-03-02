/*
** EPITECH PROJECT, 2023
** mini_printf
** File description:
** print hexa
*/
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include "include/my.h"

int print_arg(char flag, va_list *ap)
{
    if (flag == 's') {
        my_putstr(va_arg(*ap, char *));
    }
    if ((flag == 'd') || (flag == 'i')) {
        my_put_nbr_base(va_arg(*ap, int), 10);
    }
    if (flag == 'c') {
        my_putchar(va_arg(*ap, int));
    }
    if (flag == '%') {
        my_putchar('%');
    }
    if (flag == 'x') {
        my_put_nbr_base(va_arg(*ap, int), 16);
    }
    if (flag == 'o') {
        my_put_nbr_base(va_arg(*ap, int), 8);
    }
    return (0);
}

int mini_printf(const char *format, ...)
{
    int i = 0;
    va_list ap;

    va_start(ap, format);
    if (format == NULL) {
        return (-1);
    }
    while (format[i] != '\0') {
        if (format[i] == '%') {
            print_arg(format[i + 1], &ap);
            i = i + 1;
        } else {
            my_putchar(format[i]);
        }
        i = i + 1;
    }
    va_end(ap);
    return (0);
}
