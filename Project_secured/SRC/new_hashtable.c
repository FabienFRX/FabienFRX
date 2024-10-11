/*
** EPITECH PROJECT, 2024
** new_hashtable
** File description:
** create our own hashtbale
*/

#include <stdlib.h>
#include <stdio.h>
#include "../include/hashtable.h"
#include "../include/my.h"

hashtable_t *new_hashtable(int (*hash)(char *, int), int len)
{
    hashtable_t *new = malloc(sizeof(hashtable_t));

    new->len = len;
    new->table = malloc(sizeof(node_t *) * len);
    new->hash = hash;
    if (new == NULL) {
        return NULL;
    }
    if (new->table == NULL) {
        return NULL;
    }
    if (new->hash == NULL) {
        return NULL;
    }
    for (int i = 0; i < len; i++) {
        new->table[i] = NULL;
    }
    return new;
}
