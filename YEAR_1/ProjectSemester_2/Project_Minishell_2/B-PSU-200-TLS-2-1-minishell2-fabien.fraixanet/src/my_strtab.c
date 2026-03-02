/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_strtab
*/

#include "my_lib_incl.h"

void skip_initial_separators(char *str, int *i, char sep)
{
    while (str[*i] && (str[*i] == ' ' || str[*i] == sep))
        (*i)++;
}

void skip_separators(char *str, int *i, char sep)
{
    while (str[*i] && str[*i] == sep)
        (*i)++;
}

static int count_size_word(char *str, int i, char sep)
{
    int nb = 1;

    skip_separators(str, &i, sep);
    while (str[i] && str[i] != sep) {
        i++;
        nb++;
    }
    return nb;
}

static int count_sep(char *str, char c)
{
    int nb = 1;
    int i = 0;

    skip_initial_separators(str, &i, c);
    for (; str[i]; i++) {
        if (str[i] == c)
            nb++;
        skip_separators(str, &i, c);
    }
    return nb;
}

void fill_result(char *str, char sep, char **result, int *i)
{
    int j = 0;
    int k = 0;

    while (str[*i]) {
        if (str[*i] == sep && str[*i + 1] != sep && str[*i + 1]) {
            j++;
            result[j] = my_calloc_str(count_size_word(str, *i, sep));
            k = 0;
        } else {
            (str[*i] != sep);
            result[j][k] = str[*i];
            k++;
        }
        (*i)++;
    }
}

char **my_strtab(char *str, char sep)
{
    int sep_count = count_sep(str, sep);
    char **result = my_calloc_ar(sep_count);
    int i = 0;

    replace_char(str, '\t', ' ');
    skip_initial_separators(str, &i, sep);
    result[0] = my_calloc_str(count_size_word(str, i, sep));
    fill_result(str, sep, result, &i);
    return result;
}
