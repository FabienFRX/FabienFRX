/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD04pm-fabien.fraixanet
** File description:
** stack
*/

#include "stack.h"
#include "list.h"
#include <stdio.h>

unsigned int stack_get_size(stack_t *stack)
{
    return list_get_size(stack);
}

bool stack_push(stack_t **stack_ptr, void *elem)
{
    return list_add_elem_at_front(stack_ptr, elem);
}

bool stack_pop(stack_t **stack_ptr)
{
    return list_del_elem_at_front(stack_ptr);
}

void *stack_top(stack_t *stack)
{
    return list_get_elem_at_front(stack);
}
