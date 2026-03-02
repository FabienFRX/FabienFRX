/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** replace_char
*/

#include "my_lib_incl.h"

void replace_chars_in_string(char *str, char find, char replace)
{
    for (int i = 0; str[i]; i++) {
        if (str[i] == find)
            str[i] = replace;
    }
}

void replace_char(char *str, char find, char replace)
{
    if (str == NULL)
        return;
    replace_chars_in_string(str, find, replace);
}
