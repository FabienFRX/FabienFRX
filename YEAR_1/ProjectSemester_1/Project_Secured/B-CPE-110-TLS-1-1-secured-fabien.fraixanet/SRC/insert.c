/*
** EPITECH PROJECT, 2024
** B-CPE-110-TLS-1-1-secured-fabien.fraixanet
** File description:
** insert
*/

#include <stdlib.h>
#include <stdio.h>
#include "../include/hashtable.h"
#include "../include/my.h"

int ht_insert(hashtable_t *ht, char *key, char *value)
{
    int hash_res;
    node_t *after;

    if (key == NULL || value == NULL) {
        return NULL;
    }
    hash_res = ht->hash(key, my_strlen(key));
    after = malloc(sizeof(node_t));
    if (after == NULL) {
        return NULL;
    }
    after->key = hash_res;
    after->value = my_strdup(value);
    for (int i = 0; i < ht->len; i++) {
        if (ht->table[i] == NULL) {
            ht->table[i] = after;
            break;
        }
    }
    return (0);
}
