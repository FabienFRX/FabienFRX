/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-FASTAtools-fabien.fraixanet
** File description:
** option_reverse_complement
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

char *my_revstr(char *str)
{
    int first = 0;
    int last = strlen(str) - 1;
    char tmp = 0;

    while (first < last) {
        tmp = str[first];
        str[first] = str[last];
        str[last] = tmp;
        first = first + 1;
        last = last - 1;
    }
    return (str);
}

/*MISSED: T face A and G face C*/
static void display_rev_value(char *value)
{
    for (int i = 0; value[i] != '\0'; i++) {
        switch (value[i]) {
            case 'T':
                value[i] = 'A';
                break;
            case 'A':
                value[i] = 'T';
                break;
            case 'G':
                value[i] = 'C';
                break;
            case 'C':
                value[i] = 'G';
                break;
            default:
                break;
        }
    }
    display_adn(value);
}

/*display string in reverse (partir de strlen et descendre jusqu a 0 inclu)*/
int option_3(sequence_t *seq)
{
    for (; seq != NULL; seq = seq->next) {
        printf("%s\n", seq->key);
        if (seq->value != NULL) {
            display_rev_value(my_revstr(seq->value));
        }
    }
    return (0);
}
