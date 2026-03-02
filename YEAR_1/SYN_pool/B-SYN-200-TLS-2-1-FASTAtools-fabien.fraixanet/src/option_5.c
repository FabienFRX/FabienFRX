/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-FASTAtools-fabien.fraixanet
** File description:
** option_5
*/

#include "../include/syn.h"

char *substr(char *str, int begin, int length)
{
    char *dest = malloc(sizeof(char) * (length + 1));

    if (dest == NULL)
        return (NULL);
    if (length > 0) {
        dest = strncpy(dest, str + begin, length);
        dest[length] = '\0';
    }
    return dest;
}

static int ending_codons(char *tmp)
{
    if (tmp != NULL && ((strcmp(tmp, "TAA") == 0) ||
        strcmp(tmp, "TAG") == 0 || strcmp(tmp, "TGA") == 0)) {
        return (0);
    }
    return (-1);
}

static void display_adn(char *value)
{
    for (int i = 0; value[i] != '\0'; i++) {
        if (value[i] == 'A' || value[i] == 'T' ||
            value[i] == 'C' || value[i] == 'G' ||
            value[i] == 'N')
            putchar(value[i]);
    }
    putchar('\n');
}

static int get_next_substr_idx(char *value, int begin)
{
    for (int i = begin; value[i] != '\0'; i++) {
        if (value[i] == ' ')
            return (i);
    }
    return (-1);
}

static void make_iteration(char *value)
{
    int start = -1;
    int end = -1;
    char *tmp = NULL;

    for (int i = 0; value[i] != '\0'; i++) {
        if (strlen(value) - i > 3)
            tmp = substr(value, i, 3);
        if (tmp != NULL && (strcmp(tmp, "ATG") == 0) && start == -1)
            start = i;
        if (start != -1 && ending_codons(tmp) == 0)
            end = i;
        if (start != -1 && end != -1) {
            display_adn(substr(value, start, end - start));
            start = -1;
            end = -1;
        }
    }
}

static void loop_value(char *value)
{
    char *tmp = NULL;
    int next_sub = get_next_substr_idx(value, 0);

    if (next_sub == -1) {
        make_iteration(value);
        return;
    }
    for (int i = 0; i != -1 && i < strlen(value); i = next_sub) {
        if (next_sub != -1) {
            i++;
            next_sub = get_next_substr_idx(value, i);
            tmp = substr(value, i, next_sub - i);
        }
        if (next_sub == -1) {
            next_sub = strlen(value);
            tmp = substr(value, i, next_sub - i);
        }
        if (tmp != NULL)
            make_iteration(tmp);
    }
}

int option_5(sequence_t *seq)
{
    for (; seq != NULL; seq = seq->next) {
        if (seq->value != NULL)
            loop_value(seq->value);
    }
    return (0);
}
