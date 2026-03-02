/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD04am-fabien.fraixanet
** File description:
** print
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "print.h"

void print_normal(const char *str)
{
    printf("%s\n", str);
}

void print_reverse(const char *str)
{
    int len = strlen(str);

    for (int i = len - 1; i >= 0; i--) {
        putchar(str[i]);
    }
    putchar('\n');
}

void print_upper(const char *str)
{
    while (*str) {
        putchar(toupper(*str));
        str++;
    }
    putchar('\n');
}

void print_42(const char *str)
{
    (void)str;
    printf("42\n");
}

void do_action(action_t action, const char *str)
{
    void (*actions[])(const char *) = {print_normal, print_reverse,
        print_upper, print_42};

    actions[action](str);
}
