/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-FASTAtools-fabien.fraixanet
** File description:
** option_6
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

void display_amino(char *value)
{
    for (int i = 0; value[i] != '\0'; i += 3) {
        if (value[i] == "ATG")
            putchar('M');
        if (value[i] == "GCT" || value[i] == "GCC" || value[i] == "GCA" ||
            value[i] == "GCG")
            putchar('A');
        if (value[i] == "TGT" || value[i] == "TGC")
            putchar('C');
        if (value[i] == "GAT" || value[i] == "GAC")
            putchar('D');
        if (value[i] == "GAA" || value[i] == "GAG")
            putchar('E');
        if (value[i] == "TTT" || value[i] == "TTC")
            putchar('F');
        if (value[i] == "GGT" || value[i] == "GGC" || value[i] == "GGA" ||
            value[i] == "GGG")
            putchar('G');
    }
    display_adn(value);
}

void display_amino2(char *value)
{
    for (int i = 0; value[i] != '\0'; i += 3) {
        if (value[i] == "CAT" || value[i] == "CAC")
            putchar('H');
        if (value[i] == "ATT" || value[i] == "ATC" || value[i] == "ATA")
            putchar('I');
        if (value[i] == "AAA" || value[i] == "AAG")
            putchar('K');
        if (value[i] == "TTA" || value[i] == "TTG" || value[i] == "CTT" ||
            value[i] == "CTC" || value[i] == "CTA" || value[i] == "CTG")
            putchar('L');
        if (value[i] == "AAT" || value[i] == "AAC")
            putchar('N');
        if (value[i] == "CCT" || value[i] == "CCC" || value[i] == "CCA" ||
            value[i] == "CCG")
            putchar('P');
    }
    display_adn(value);
}

void display_amino3(char *value)
{
    for (int i = 0; value[i] != '\0'; i += 3) {
        if (value[i] == "CAA" || value[i] == "CAG")
            putchar('Q');
        if (value[i] == "AGA" || value[i] == "AGG" || value[i] == "CGT" ||
            value[i] == "CGC" || value[i] == "CGA" || value[i] == "CGG")
            putchar('R');
        if (value[i] == "TCT" || value[i] == "TCC" || value[i] == "TCA" ||
            value[i] == "TCG" || value[i] == "AGT" || value[i] == "AGC")
            putchar('S');
        if (value[i] == "ACT" || value[i] == "ACC" || value[i] == "ACA" ||
            value[i] == "ACG")
            putchar('T');
        if (value[i] == "GTT" || value[i] == "GTC" || value[i] == "GTA" ||
            value[i] == "GTG")
            putchar('V');
    }
    display_adn(value);
}

void display_amino4(char *value)
{
    for (int i = 0; value[i] != '\0'; i += 3) {
        if (value[i] == "TGG")
            putchar('W');
        if (value[i] == "TAT" || value[i] == "TAC")
            putchar('Y');
    }
    display_adn(value);
}

static int ending_codons(char *tmp)
{
    if (tmp != NULL && ((strcmp(tmp, "TAA") == 0) ||
        strcmp(tmp, "TAG") == 0 || strcmp(tmp, "TGA") == 0)) {
        return (0);
    }
    return (-1);
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

int option_6(sequence_t *seq)
{
    for (; seq != NULL; seq = seq->next) {
        if (seq->value != NULL)
            loop_value(seq->value);
    }
    return (0);
}
