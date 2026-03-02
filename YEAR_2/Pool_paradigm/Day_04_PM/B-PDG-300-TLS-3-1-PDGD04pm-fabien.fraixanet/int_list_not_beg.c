/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD04pm-fabien.fraixanet
** File description:
** int_list_not_beg
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "int_list.h"

bool int_list_add_elem_at_front(int_list_t **front_ptr, int elem)
{
    int_list_t *new_node = malloc(sizeof(int_list_t));

    if (!new_node)
        return false;
    new_node->value = elem;
    new_node->next = *front_ptr;
    *front_ptr = new_node;
    return true;
}

bool int_list_add_elem_at_position(int_list_t **front_ptr, int elem,
    unsigned int position)
{
    int_list_t *new_node = malloc(sizeof(int_list_t));
    int_list_t *current = *front_ptr;

    if (!new_node)
        return false;
    new_node->value = elem;
    if (position == 0) {
        new_node->next = *front_ptr;
        *front_ptr = new_node;
        return true;
    }
    for (unsigned int i = 0; current && i < position - 1; i++) {
        current = current->next;
    }
    if (!current)
        return false;
    new_node->next = current->next;
    current->next = new_node;
    return true;
}
