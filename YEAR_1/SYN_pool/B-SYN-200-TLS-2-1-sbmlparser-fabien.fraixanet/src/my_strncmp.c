/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-sbmlparser-fabien.fraixanet
** File description:
** my_strncmp
*/

#include "../include/syn.h"

int my_strncmp(char const *s1, char const *s2, int n)
{
    int i = 0;

    while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0' && i < n) {
        i = i + 1;
    }
    if ((s1[i] == '\0' && s2[i] == '\0') || i == n) {
        return (0);
    }
    if (s1[i] > s2[i]) {
        return (1);
    }
    return (-1);
}
