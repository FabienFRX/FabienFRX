/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-sbmlparser-fabien.fraixanet
** File description:
** requirement
*/

#include <stdio.h>
#include <stdlib.h>

static int is_alph_num(char c)
{
    if (((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z')))
        return (0);
    return (1);
}

static int my_strlen(char const *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    return (i);
}

static int get_nb_words(char const *str)
{
    int nb_words = 0;

    if (str == NULL)
        return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (is_alph_num(str[i]) == 1)
            nb_words++;
    }
    return nb_words;
}

static int get_next_word_size(char const *str, int begin)
{
    int size = 0;

    for (int i = begin; str[i] != '\0'; i++) {
        if (is_alph_num(str[i]) == 1) {
            break;
        } else {
            size++;
        }
    }
    return size;
}

static int copy_word(char const *src, char *dest, int begin, int size)
{
    int j = 0;

    for (j = begin; src[j] != '\0' && j < (size + begin); j++) {
        dest[j - begin] = src[j];
    }
    dest[j - begin] = '\0';
    return j - begin;
}

char **my_str_to_word_array_synthesis(char const *str)
{
    int i = 0;
    int word_len = 0;
    int count_words = 0;
    char **words_array = malloc(sizeof(char *) * (get_nb_words(str) + 1));

    while (str[i] != '\0') {
        word_len = get_next_word_size(str, i);
        if (word_len) {
            words_array[count_words] = malloc(sizeof(char) * (word_len + 1));
            i += copy_word(str, words_array[count_words], i, word_len);
            count_words += 1;
        } else {
            i += 1;
        }
    }
    words_array[count_words] = NULL;
    return (words_array);
}
