/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_strlen
*/

#include "my_lib_incl.h"

int count_characters(char *str)
{
    int nb = 0;

    while (str[nb]) {
        nb++;
    }
    return nb;
}

int my_strlen(char *str)
{
    if (str == NULL)
        return 0;
    return count_characters(str);
}
