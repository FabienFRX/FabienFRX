/*
** EPITECH PROJECT, 2023
** my_strlen
** File description:
** counts and returns the number of char in array passed in parameters
*/

#include <unistd.h>

int my_arrlen(char **tab)
{
    int i = 0;

    while (tab[i] != NULL) {
        i = i + 1;
    }
    return (i);
}
