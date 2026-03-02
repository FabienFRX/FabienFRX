/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_strncmp
*/

#include "my_lib_incl.h"

int compare_strings(char *str1, char *str2, int n)
{
    for (int i = 0; i < n; i++) {
        if (str1[i] != str2[i])
            return -1;
    }
    return 0;
}

int validate_length(char *str1, char *str2, int n)
{
    if (my_strlen(str1) < n || my_strlen(str2) < n)
        return -1;
    return 0;
}

int my_strncmp(char *str1, char *str2, int n)
{
    if (validate_length(str1, str2, n) != 0)
        return -1;
    return compare_strings(str1, str2, n);
}
