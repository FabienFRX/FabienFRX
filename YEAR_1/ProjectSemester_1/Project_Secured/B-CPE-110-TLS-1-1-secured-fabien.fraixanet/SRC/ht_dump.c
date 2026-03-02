/*
** EPITECH PROJECT, 2024
** ht_dump
** File description:
** display the current state of the table
*/

#include <stdlib.h>
#include <stdio.h>
#include "../include/hashtable.h"
#include "../include/my.h"

void ht_dump(hashtable_t *ht)
{
    for (int i = 0; i < ht->len; i++) {
        my_printf("[%d]:\n", i);
        if (ht->table[i] == NULL) {
            continue;
        }
        my_printf("> %i - %s\n", ht->table[i]->key, ht->table[i]->value);
    }
}
