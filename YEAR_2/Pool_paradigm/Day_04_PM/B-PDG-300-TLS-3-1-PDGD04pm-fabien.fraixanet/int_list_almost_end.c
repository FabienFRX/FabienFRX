/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD04pm-fabien.fraixanet
** File description:
** int_list_almost_end
*/

#include <stdio.h>
#include <stdlib.h>
#include "int_list.h"

int int_list_get_elem_at_front(int_list_t *list)
{
    if (!list)
        return 0;
    return list->value;
}

int int_list_get_elem_at_back(int_list_t *list)
{
    if (!list)
        return 0;
    while (list->next) {
        list = list->next;
    }
    return list->value;
}

int int_list_get_elem_at_position(int_list_t *list, unsigned int position)
{
    if (!list)
        return 0;
    for (unsigned int i = 0; i < position; i++) {
        if (!list->next)
            return 0;
        list = list->next;
    }
    return list->value;
}
