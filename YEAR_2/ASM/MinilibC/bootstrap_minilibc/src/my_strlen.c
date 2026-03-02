/*
** EPITECH PROJECT, 2025
** MinilibC
** File description:
** my_strlen
*/

#include <unistd.h>

size_t my_strlen(const char *str)
{
    size_t len = 0;

    while (str[len])
        len++;
    return len;
}
