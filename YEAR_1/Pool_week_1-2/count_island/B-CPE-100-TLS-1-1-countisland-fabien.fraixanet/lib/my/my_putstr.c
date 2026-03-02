/*
** EPITECH PROJECT, 2023
** my_putstr
** File description:
** my_putstr pour lib
*/
#include "../../include/my.h"

int my_putstr(char const *str)
{
    int i = 0;

    while (str[i] != '\0') {
        my_putchar(str[i]);
        i = i +1;
    }
    return (0);
}
