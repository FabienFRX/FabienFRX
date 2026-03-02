/*
** EPITECH PROJECT, 2023
** my_putstr
** File description:
** display one by one the char of a string
*/
#include "../../include/my.h"
#include <unistd.h>

int my_putstr(char const *str)
{
    int i = 0;

    while (str[i] != '\0') {
        i = i +1;
    }
    write(1, str, i);
    return (0);
}
