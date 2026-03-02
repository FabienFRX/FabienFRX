/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD04pm-fabien.fraixanet
** File description:
** queue1
*/

#include "queue.h"

bool queue_is_empty(queue_t *queue)
{
    return list_is_empty(queue);
}

void queue_clear(queue_t **queue_ptr)
{
    list_clear(queue_ptr);
}
