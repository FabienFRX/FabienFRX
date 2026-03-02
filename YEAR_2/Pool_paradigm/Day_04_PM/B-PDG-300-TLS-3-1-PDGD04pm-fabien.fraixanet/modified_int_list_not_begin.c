/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD04pm-fabien.fraixanet
** File description:
** modified_int_list_not_begin
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

bool list_add_elem_at_front(list_t **front_ptr, void *elem)
{
    list_t *new_node = malloc(sizeof(list_t));

    if (!new_node)
        return false;
    new_node->value = elem;
    new_node->next = *front_ptr;
    *front_ptr = new_node;
    return true;
}

bool list_add_elem_at_position(list_t **front_ptr, void *elem,
    unsigned int pos)
{
    list_t *current = *front_ptr;
    list_t *new_node = malloc(sizeof(list_t));

    if (pos == 0)
        return list_add_elem_at_front(front_ptr, elem);
    if (!new_node)
        return false;
    new_node->value = elem;
    for (unsigned int i = 0; i < pos - 1; i++) {
        if (!current)
            return false;
        current = current->next;
    }
    new_node->next = current->next;
    current->next = new_node;
    return true;
}
