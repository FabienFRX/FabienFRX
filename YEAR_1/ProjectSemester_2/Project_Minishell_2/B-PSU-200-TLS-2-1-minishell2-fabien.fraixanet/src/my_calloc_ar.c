/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_calloc_ar
*/

#include "my_lib_incl.h"

void initialize_array(char **array, int size)
{
    for (int i = 0; i <= size; i++) {
        array[i] = NULL;
    }
}

char **my_calloc_ar(int size)
{
    char **result = malloc(sizeof(char *) * (size + 1));

    if (result == NULL) {
        write_my_error("Impossible to malloc");
        return NULL;
    }
    initialize_array(result, size);
    return result;
}
