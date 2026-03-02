/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD05-fabien.fraixanet
** File description:
** test_main
*/

#include <stdio.h>
#include "string.h"


int main(void)
{
    string_t s;

    string_init(&s, "12345");
    printf("String content: %s\n", s.str);
    printf("Converted to int: %d\n", s.to_int(&s)); // Affiche 12345

    s.assign_c(&s, "-6789");
    printf("String content: %s\n", s.str);
    printf("Converted to int: %d\n", s.to_int(&s)); // Affiche -6789

    s.assign_c(&s, "abc123");
    printf("String content: %s\n", s.str);
    printf("Converted to int: %d\n", s.to_int(&s)); // Affiche 0 (pas un nombre valide)

    string_destroy(&s);
    return 0;
}