/*
** EPITECH PROJECT, 2023
** MY_PRINTF.H
** File description:
** all prototypes
*/
#include <stdarg.h>
#ifndef MY_PRINTF_H
    #define MY_PRINTF_H
typedef unsigned char byte_t;
void print_p(void *ptr);
int print_arg(char flag, va_list *ap);
int print_arg2(char flag, va_list *ap);
int print_arg3(char flag, va_list *ap);
int print_all(const char *format, int i, va_list *ap);
int my_printf(const char *format, ...);
void my_print_float(double f, int decimals);
int my_put_nbr_u(int nb);
#endif /*MY_PRINTF_H*/
