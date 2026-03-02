/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_strcmp
*/

#include "my_lib_incl.h"

int compare_lengths(char *str1, char *str2)
{
    if (my_strlen(str1) != my_strlen(str2))
        return -1;
    return 0;
}

int compare_chars(char *str1, char *str2)
{
    for (int i = 0; str1[i]; i++) {
        if (str1[i] != str2[i])
            return -1;
    }
    return 0;
}

int my_strcmp(char *str1, char *str2)
{
    if (compare_lengths(str1, str2) != 0)
        return -1;
    return compare_chars(str1, str2);
}
