/*
** EPITECH PROJECT, 2024
** MY_STR_TO_WORDTAB
** File description:
** qwerty
*/
#include <stdlib.h>
#include "../../include/my.h"

static int count_occurrences(char *buffer, char letter)
{
    int count = 0;

    for (int i = 0; buffer[i] != '\0'; i++)
        if (buffer[i] == letter)
            count++;
    return count;
}

static char *extract_word(char *buffer, int start, int end)
{
    int length = end - start;
    char *word = malloc(sizeof(char) * (length + 1));

    if (word == NULL)
        return NULL;
    for (int i = 0; i < length; i++)
        word[i] = buffer[start + i];
    word[length] = '\0';
    return word;
}

char **my_str_to_wordtab(char *buffer, char letter)
{
    int count_tab = count_occurrences(buffer, letter);
    int count = 0;
    char **tab = NULL;
    int end;
    int start;

    tab = malloc(sizeof(char *) * (count_tab + 2));
    for (int i = 0; i < count_tab + 1; i++) {
        while (buffer[count] == letter)
            count++;
        start = count;
        while (buffer[count] != letter && buffer[count] != '\0')
            count++;
        end = count;
        tab[i] = extract_word(buffer, start, end);
    }
    tab[count_tab + 1] = NULL;
    return tab;
}
