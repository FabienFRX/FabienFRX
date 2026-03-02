/*
** EPITECH PROJECT, 2024
** ht_search
** File description:
** search
*/

#include <stdlib.h>
#include <stdio.h>
#include "../include/hashtable.h"
#include "../include/my.h"

char *ht_search(hashtable_t *ht, char *k)
{
    int hash_res = hash(k, my_strlen(k));

    if (ht == NULL || k == NULL) {
        return NULL;
    }
    for (int i = 0; i < ht->len; i++) {
        if (ht->table[i] != NULL && ht->table[i]->key == hash_res) {
            return (ht->table[i]->value);
        }
    }
    return (NULL);
}
