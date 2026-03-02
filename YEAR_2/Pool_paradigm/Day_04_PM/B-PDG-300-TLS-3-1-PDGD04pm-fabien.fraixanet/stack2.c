/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD04pm-fabien.fraixanet
** File description:
** stack2
*/

#include "stack.h"
#include "list.h"

bool stack_is_empty(stack_t *stack)
{
    return list_is_empty(stack);
}

void stack_clear(stack_t **stack_ptr)
{
    list_clear(stack_ptr);
}
