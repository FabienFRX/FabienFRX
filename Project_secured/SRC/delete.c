/*
** EPITECH PROJECT, 2024
** B-CPE-110-TLS-1-1-secured-fabien.fraixanet
** File description:
** delete
*/

#include <stdlib.h>
#include <stdio.h>
#include "../include/hashtable.h"
#include "../include/my.h"

int ht_delete(hashtable_t *ht, char *key)
{
    int hash_res = hash(key, my_strlen(key));

    for (int i = 0; i < ht->len; i++) {
        if (ht->table[i] != NULL && ht->table[i]->key == hash_res) {
            ht->table[i] = NULL;
        }
    }
    return (0);
}
