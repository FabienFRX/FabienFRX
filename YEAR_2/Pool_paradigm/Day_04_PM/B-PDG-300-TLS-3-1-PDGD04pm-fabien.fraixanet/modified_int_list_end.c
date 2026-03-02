/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD04pm-fabien.fraixanet
** File description:
** modified_int_list_end
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

bool list_del_elem_at_front(list_t **front_ptr)
{
    list_t *temp = *front_ptr;

    if (*front_ptr == NULL)
        return false;
    *front_ptr = (*front_ptr)->next;
    free(temp);
    return true;
}

bool list_del_elem_at_back(list_t **front_ptr)
{
    list_t *current = *front_ptr;

    if (*front_ptr == NULL)
        return false;
    if ((*front_ptr)->next == NULL) {
        free(*front_ptr);
        *front_ptr = NULL;
        return true;
    }
    while (current->next && current->next->next)
        current = current->next;
    free(current->next);
    current->next = NULL;
    return true;
}

bool list_del_elem_at_position(list_t **front_ptr, unsigned int position)
{
    list_t *current = *front_ptr;
    list_t *temp = current->next;

    if (position == 0)
        return list_del_elem_at_front(front_ptr);
    for (unsigned int i = 0; i < position - 1; i++) {
        if (!current)
            return false;
        current = current->next;
    }
    if (!current || !current->next)
        return false;
    current->next = current->next->next;
    free(temp);
    return true;
}
