/*
** EPITECH PROJECT, 2023
** my_strncpy
** File description:
** copies n characters of a string into another string,
** both passed in parameters
*/
#include <stdio.h>

int my_strlen(char const *str)
{
    int i = 0;

    while (str[i] != '\0') {
        i += 1;
    }
    return (i);
}

char *my_strncpy(char *dest, char const *src, int n)
{
    int i = 0;
    int len_dest = my_strlen(dest);

    while (i < n) {
        dest[i] = src[i];
        i += 1;
    }
    if (len_dest < n) {
        dest[n] = '\0';
    }
    return (dest);
}
