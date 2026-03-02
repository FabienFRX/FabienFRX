/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-calendar-fabien.fraixanet
** File description:
** requirement
*/

#include <stdlib.h>
#include "requirement.h"

int my_revlist_synthesis(list_t **begin)
{
    list_t *prev = NULL;
    list_t *current = *begin;
    list_t *next = NULL;

    if (begin == NULL || *begin == NULL) {
        return 0;
    }
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *begin = prev;
    return 0;
}

list_t *my_findnodeeqinlist_synthesis(list_t *begin, void *data_ref,
    int (*cmp)(void *, void *))
{
    while (begin != NULL) {
        if (cmp(begin->data, data_ref) == 0) {
            return begin;
        }
        begin = begin->next;
    }
    return NULL;
}
