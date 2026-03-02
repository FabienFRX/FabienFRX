/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD04pm-fabien.fraixanet
** File description:
** map1
*/

#include "map.h"
#include "list.h"
#include <stdlib.h>
#include <stdbool.h>

unsigned int map_get_size(map_t *map)
{
    return list_get_size(map);
}

bool map_is_empty(map_t *map)
{
    return list_is_empty(map);
}

void map_clear(map_t **map_ptr)
{
    map_t *current = *map_ptr;
    map_t *temp = current;
    pair_t *pair = (pair_t *)current->value;

    if (!map_ptr || !*map_ptr)
        return;
    while (current) {
        free(pair);
        current = current->next;
        free(temp);
    }
    *map_ptr = NULL;
}
