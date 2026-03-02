/*
** EPITECH PROJECT, 2024
** B-CPE-110-TLS-1-1-secured-fabien.fraixanet
** File description:
** hash
*/

#include <stdlib.h>
#include <stdio.h>
#include "../include/hashtable.h"
#include "../include/my.h"

int hash(char *key, int len)
{
    int hash = 0;

    for (int i = 0; i < len; i++) {
        if (hash < 0)
            hash *= -1;
        hash = (int) key[i] + (hash << 8) + (hash << 16) - hash;
    }
    return hash;
}
