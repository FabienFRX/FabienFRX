/*
** EPITECH PROJECT, 2024
** B-CPE-110-TLS-1-1-secured-fabien.fraixanet
** File description:
** delete_hastable
*/

#include <stdlib.h>
#include <stdio.h>
#include "../include/hashtable.h"
#include "../include/my.h"

void delete_hashtable(hashtable_t *ht)
{
    for (int i = 0; i < ht->len; i++) {
        if (ht->table[i] != NULL) {
            free(ht->table[i]->value);
            free(ht->table[i]);
        }
    }
    free(ht->table);
    free(ht);
    return;
}
