/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD04pm-fabien.fraixanet
** File description:
** int_list
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "int_list.h"

bool int_list_add_elem_at_back(int_list_t **front_ptr, int elem)
{
    int_list_t *new_node = malloc(sizeof(int_list_t));
    int_list_t *temp = *front_ptr;

    if (!new_node)
        return false;
    new_node->value = elem;
    new_node->next = NULL;
    if (*front_ptr == NULL) {
        *front_ptr = new_node;
        return true;
    }
    while (temp->next) {
        temp = temp->next;
    }
    temp->next = new_node;
    return true;
}

void int_list_dump(int_list_t *list)
{
    for (int_list_t *temp = list; temp != NULL; temp = temp->next) {
        printf("%d\n", temp->value);
    }
}

unsigned int int_list_get_size(int_list_t *list)
{
    unsigned int size = 0;

    for (int_list_t *temp = list; temp != NULL; temp = temp->next) {
        size++;
    }
    return size;
}

bool int_list_is_empty(int_list_t *list)
{
    return list == NULL;
}

void int_list_clear(int_list_t **front_ptr)
{
    int_list_t *temp = *front_ptr;

    while (*front_ptr) {
        temp = *front_ptr;
        *front_ptr = (*front_ptr)->next;
        free(temp);
    }
}
