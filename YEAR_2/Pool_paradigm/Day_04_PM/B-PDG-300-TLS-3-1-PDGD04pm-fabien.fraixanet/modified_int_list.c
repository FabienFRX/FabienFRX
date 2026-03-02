/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD04pm-fabien.fraixanet
** File description:
** modified_int_list
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

bool list_add_elem_at_back(list_t **front_ptr, void *elem)
{
    list_t *new_node = malloc(sizeof(list_t));
    list_t *current = *front_ptr;

    if (!new_node)
        return false;
    new_node->value = elem;
    new_node->next = NULL;
    if (*front_ptr == NULL) {
        *front_ptr = new_node;
        return true;
    }
    while (current->next) {
        current = current->next;
    }
    current->next = new_node;
    return true;
}

void list_dump(list_t *list, value_displayer_t val_disp)
{
    if (list == NULL) {
        printf("The list is empty.\n");
        return;
    }
    while (list) {
        val_disp(list->value);
        printf("\n");
        list = list->next;
    }
}

unsigned int list_get_size(list_t *list)
{
    unsigned int size = 0;

    while (list) {
        size++;
        list = list->next;
    }
    return size;
}

bool list_is_empty(list_t *list)
{
    return list == NULL;
}

void list_clear(list_t **front_ptr)
{
    while (*front_ptr) {
        list_del_elem_at_front(front_ptr);
    }
}
