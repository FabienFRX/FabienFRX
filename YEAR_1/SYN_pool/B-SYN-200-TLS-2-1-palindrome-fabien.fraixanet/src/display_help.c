/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-palindrome-fabien.fraixanet
** File description:
** display_help
*/

#include "../include/syn.h"

/* display_help function: all print usage */
void display_help(void)
{
    printf("USAGE\n");
    printf("\t./palindrome -n number -p palindrome [-b base]");
    printf("[-imin i] [-imax i]\n");
    printf("DESCRIPTION\n");
    printf("\t-n n integer to be transformed (>=0)\n");
    printf("\t-p pal palindromic number to be obtained");
    printf("(incompatible with the -n option) (>=0)\n");
    printf("\t   if defined, all fitting values of n will be output\n");
    printf("\t-b base base in which the procedure will be executed");
    printf("(1<b<=10) [def: 10]\n");
    printf("\t-imin i minimum number of iterations,");
    printf("included (>=0) [def: 0]\n");
    printf("\t-imax i maximum number of iterations,");
    printf("included (>=0) [def: 100]\n");
}
