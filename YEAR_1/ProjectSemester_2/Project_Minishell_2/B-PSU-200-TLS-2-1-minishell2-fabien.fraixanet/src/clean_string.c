/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** clean_string
*/

#include "my_lib_incl.h"

void truncate_at_first_space(char *str)
{
    int i = 0;

    while (str[i] && str[i] != ' ') {
        i++;
    }
    if (str[i] == ' ') {
        str[i] = '\0';
    }
}

char *remove_leading_spaces(char *str)
{
    while (str[0] && str[0] == ' ') {
        str++;
    }
    return str;
}

char *clean_string(char *str)
{
    if (str == NULL)
        return NULL;
    str = remove_leading_spaces(str);
    truncate_at_first_space(str);
    return str;
}
