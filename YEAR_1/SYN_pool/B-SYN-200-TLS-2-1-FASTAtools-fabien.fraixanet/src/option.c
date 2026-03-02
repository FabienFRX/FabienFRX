/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-FASTAtools-fabien.fraixanet
** File description:
** option
*/

#include "../include/syn.h"

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

int option_1(sequence_t *seq)
{
    for (; seq != NULL; seq = seq->next) {
        printf("%s\n", seq->key);
        if (seq->value != NULL) {
            display_adn(seq->value);
        }
    }
    return (0);
}
