/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-FASTAtools-fabien.fraixanet
** File description:
** display_all_help
*/

#include "../include/syn.h"

void display_help(void)
{
    printf("USAGE\n");
    printf("    ./FASTAtools option [k]\n\n");
    printf("DESCRIPTION\n");
    printf("    option 1: read FASTA from the standard input,");
    printf(" write the DNA sequences to the \n\tstandard output\n");
    printf("    option 2: read FASTA from the standard input,");
    printf(" write the RNA sequences to the \n\tstandard output\n");
    printf("    option 3: read FASTA from the standard input,");
    printf(" write the reverse complement \n\tto the standard output\n");
    printf("    option 4: read FASTA from the standard input,");
    printf(" write the k-mer list to the \n\tstandard output\n");
    printf("    option 5: read FASTA from the standard input,");
    printf(" write the coding sequences \n\tlist to the standard output\n");
    printf("    option 6: read FASTA from the standard input,");
    printf("write the amino acids list to \n\tthe standard output\n");
    printf("    option 7: read FASTA from the standard input");
    printf(" containing exactly 2 squences,\n\t");
    printf("align them and write the result to the standard output\n");
    printf("    k: size of the k-mers for option 4\n");
}
