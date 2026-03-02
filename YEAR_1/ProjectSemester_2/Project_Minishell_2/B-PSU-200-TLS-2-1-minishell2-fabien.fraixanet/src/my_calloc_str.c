/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_calloc_str
*/

#include "my_lib_incl.h"

void initialize_string(char *str, int size)
{
    for (int i = 0; i <= size; i++) {
        str[i] = '\0';
    }
}

char *my_calloc_str(int size)
{
    char *result = malloc(sizeof(char) * (size + 1));

    if (result == NULL) {
        write_my_error("Impossible to malloc");
        return NULL;
    }
    initialize_string(result, size);
    return result;
}
