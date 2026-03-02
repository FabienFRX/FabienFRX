/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_tabdup
*/

#include "my_lib_incl.h"

void copy_tab_elements(char **dest, char **src)
{
    for (int i = 0; src[i]; i++) {
        dest[i] = my_strdup(src[i]);
    }
}

char **my_tabdup(char **str)
{
    int nb = my_strlen_array(str);
    char **result = my_calloc_ar(nb);

    if (str == NULL)
        return NULL;
    if (result == NULL)
        return NULL;
    copy_tab_elements(result, str);
    return result;
}
