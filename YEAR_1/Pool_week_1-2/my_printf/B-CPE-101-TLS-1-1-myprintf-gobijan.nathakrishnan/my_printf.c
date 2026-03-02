/*
** EPITECH PROJECT, 2023
** test
** File description:
** test
*/

#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include "include/my.h"

int print_arg(char flag, va_list *ap)
{
    switch (flag) {
    case 'o':
        my_put_nbr_base(va_arg(*ap, int), 8);
        break;
    case 'd':
    case 'i':
        my_put_nbr_base(va_arg(*ap, int), 10);
        break;
    case 'c':
        my_putchar(va_arg(*ap, int));
        break;
    case 'x':
    case 'X':
        my_put_nbr_base(va_arg(*ap, int), 16);
        break;
    case '%':
        my_putchar('%');
        break;
    }
    return (0);
}

int print_arg2(char flag, va_list *ap)
{
    switch (flag) {
    case 's':
        my_putstr(va_arg(*ap, char *));
        break;
    case 'p':
        print_p(va_arg(*ap, void *));
        break;
    case 'u':
        my_put_nbr_u(va_arg(*ap, unsigned int));
        break;
    case 'f':
        my_print_float(va_arg(*ap, double), 100);
        break;
    }
    return (0);
}

int my_printf(const char *format, ...)
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
            print_arg2(format[i + 1], &ap);
            i = i + 1;
        } else {
            my_putchar(format[i]);
        }
        i = i + 1;
    }
    va_end(ap);
    return (0);
}
