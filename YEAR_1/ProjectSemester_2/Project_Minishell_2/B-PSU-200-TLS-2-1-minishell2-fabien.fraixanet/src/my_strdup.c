/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_strdup
*/

#include "my_lib_incl.h"

void cp_string(char *dest, char *src)
{
    for (int i = 0; src[i]; i++)
        dest[i] = src[i];
    dest[my_strlen(src)] = '\0';
}

char *my_strdup(char *str)
{
    char *result = my_calloc_str(my_strlen(str) + 1);

    if (str == NULL)
        return NULL;
    if (result == NULL)
        return NULL;
    copy_string(result, str);
    return result;
}
