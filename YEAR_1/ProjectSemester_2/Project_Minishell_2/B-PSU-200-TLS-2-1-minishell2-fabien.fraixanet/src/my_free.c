/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_free
*/

#include "my_lib_incl.h"

void free_string_array(char **str)
{
    int i = 0;

    while (str[i] != NULL) {
        free(str[i]);
        i++;
    }
}

void my_free(char **str)
{
    if (str == NULL)
        return;
    free_string_array(str);
    free(str);
}
