/*
** EPITECH PROJECT, 2023
** BS_Minishell_1
** File description:
** my_str_to_word_array.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool char_is_in_str(char c, char *str)
{
    for (int i = 0; str[i] != '\0'; i++) {
        if (c == str[i]) {
            return true;
        }
    }
    return false;
}

int word_counter(char *str, char *stop)
{
    int nb_word = 0;

    if (str == NULL)
        return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (char_is_in_str(str[i], stop) == false &&
            (char_is_in_str(str[i + 1], stop) == true || str[i + 1] == '\0')) {
            nb_word++;
        }
    }
    return nb_word;
}

int sizeof_word(char *str, int *j, char *stop)
{
    int size_word = 0;

    for (; char_is_in_str(str[*j], stop) == true; *j += 1);
    for (; char_is_in_str(str[*j], stop) == false &&
        str[*j] != '\0'; *j += 1) {
        size_word++;
    }
    return size_word;
}

char **my_str_to_word_array(char *str, char *stop)
{
    int nb_word = word_counter(str, stop);
    char **w_array = malloc(sizeof(char *) * (nb_word + 1));
    int k = 0;
    int size_word;

    if (str == NULL)
        return NULL;
    for (int i = 0; i < nb_word; i++) {
        size_word = sizeof_word(str, &k, stop);
        w_array[i] = malloc(sizeof(char) * size_word + 1);
        for (int j = 0; j < size_word; j++) {
            w_array[i][j] = str[k - size_word + j];
        }
        w_array[i][size_word] = '\0';
    }
    w_array[nb_word] = NULL;
    return w_array;
}
