/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD04pm-fabien.fraixanet
** File description:
** modified_int_list_almost_end
*/

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void *list_get_elem_at_front(list_t *list)
{
    if (list)
        return list->value;
    return NULL;
}

void *list_get_elem_at_back(list_t *list)
{
    list_t *current = list;

    if (!list)
        return NULL;
    while (current->next)
        current = current->next;
    return current->value;
}

void *list_get_elem_at_position(list_t *list, unsigned int pos)
{
    for (unsigned int i = 0; i < pos && list; i++) {
        list = list->next;
    }
    if (list)
        return list->value;
    return NULL;
}
