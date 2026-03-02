/*
** EPITECH PROJECT, 2025
** MinilibC
** File description:
** my_putstr
*/

#include <unistd.h>

void my_putstr(const char *str)
{
    while (*str)
        my_putchar(*str++);
}
