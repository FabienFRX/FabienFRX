/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_strcat
*/

#include "my_lib_incl.h"

void concatenate_str(char *dest, char *src, int start)
{
    int i = start;
    int j = 0;

    while (src[j]) {
        dest[i] = src[j];
        i++;
        j++;
    }
    dest[i] = '\0';
}

int copy_str(char *dest, char *src)
{
    int i = 0;

    while (src[i]) {
        dest[i] = src[i];
        i++;
    }
    return i;
}

char *my_strcat(char *str1, char *str2)
{
    char *result = my_calloc_str(my_strlen(str1) + my_strlen(str2) + 1);
    int i = 0;

    if (str1 != NULL) {
        i = copy_string(result, str1);
    }
    if (str2 != NULL) {
        concatenate_string(result, str2, i);
    }
    return result;
}
