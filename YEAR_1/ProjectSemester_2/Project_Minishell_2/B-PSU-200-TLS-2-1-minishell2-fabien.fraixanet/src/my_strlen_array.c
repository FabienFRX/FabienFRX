/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_strlen_array
*/

#include "my_lib_incl.h"

int count_elements(char **str)
{
    int nb = 0;

    while (str[nb]) {
        nb++;
    }
    return nb;
}

int my_strlen_array(char **str)
{
    if (str == NULL)
        return 0;
    return count_elements(str);
}
