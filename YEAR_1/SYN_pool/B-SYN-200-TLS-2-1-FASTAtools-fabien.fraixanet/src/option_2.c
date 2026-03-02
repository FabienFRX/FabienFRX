/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-FASTAtools-fabien.fraixanet
** File description:
** option_rna
*/

#include "../include/syn.h"

static void display_adn(char *value)
{
    for (int i = 0; value[i] != '\0'; i++) {
        if (value[i] == 'A' || value[i] == 'U' ||
            value[i] == 'C' || value[i] == 'G' ||
            value[i] == 'N')
            putchar(value[i]);
    }
    putchar('\n');
}

/*display seq with T replace by U*/
static void display_value(char *value)
{
    for (int i = 0; value[i] != '\0'; i++) {
        if (value[i] == 'T') {
            value[i] = 'U';
        }
    }
    display_adn(value);
}

int option_2(sequence_t *seq)
{
    for (; seq != NULL; seq = seq->next) {
        printf("%s\n", seq->key);
        if (seq->value != NULL)
            display_value(seq->value);
    }
    return (0);
}
