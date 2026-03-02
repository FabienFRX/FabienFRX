/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD04pm-fabien.fraixanet
** File description:
** int_list_end
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "int_list.h"

bool int_list_del_elem_at_front(int_list_t **front_ptr)
{
    int_list_t *temp = *front_ptr;

    if (!front_ptr || !*front_ptr)
        return false;
    *front_ptr = (*front_ptr)->next;
    free(temp);
    return true;
}

bool int_list_del_elem_at_back(int_list_t **front_ptr)
{
    int_list_t *current = *front_ptr;

    if (!front_ptr || !*front_ptr)
        return false;
    if (!current->next) {
        free(current);
        *front_ptr = NULL;
        return true;
    }
    while (current->next->next) {
        current = current->next;
    }
    free(current->next);
    current->next = NULL;
    return true;
}

bool int_list_del_elem_at_position(int_list_t **front_ptr,
    unsigned int position)
{
    int_list_t *current = *front_ptr;
    int_list_t *temp = NULL;

    if (!front_ptr || !*front_ptr)
        return false;
    if (position == 0)
        return int_list_del_elem_at_front(front_ptr);
    for (unsigned int i = 0; i < position - 1; i++) {
        if (!current->next)
            return false;
        current = current->next;
    }
    if (!current->next)
        return false;
    temp = current->next;
    current->next = current->next->next;
    free(temp);
    return true;
}
