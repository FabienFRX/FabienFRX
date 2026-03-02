/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_strcat_path
*/

#include "my_lib_incl.h"

void concatenate_string(char *dest, char *src, int start)
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

int copy_string(char *dest, char *src)
{
    int i = 0;

    while (src[i]) {
        dest[i] = src[i];
        i++;
    }
    return i;
}

char *my_strcat_path(char *str1, char *str2)
{
    char *result = my_calloc_str(my_strlen(str1) + my_strlen(str2) + 2);
    int i = 0;

    if (str1 != NULL) {
        i = copy_string(result, str1);
    }
    if (i != 0 && result[i - 1] != '/') {
        result[i] = '/';
        i++;
    }
    if (str2 != NULL) {
        concatenate_string(result, str2, i);
    }
    return result;
}
