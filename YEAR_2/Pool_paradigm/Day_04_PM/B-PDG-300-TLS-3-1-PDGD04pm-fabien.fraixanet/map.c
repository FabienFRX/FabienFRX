/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD04pm-fabien.fraixanet
** File description:
** map
*/

#include "map.h"
#include "list.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

bool map_add_elem(map_t **map_ptr, void *key, void *value,
    key_comparator_t key_cmp)
{
    map_t *new_node = malloc(sizeof(map_t));
    pair_t *new_pair = malloc(sizeof(pair_t));
    map_t *current = *map_ptr;
    pair_t *current_pair = (pair_t *)current->value;

    if (!map_ptr || !key_cmp)
        return false;
    if (!new_pair)
        return false;
    new_pair->key = key;
    new_pair->value = value;
    while (current) {
        if (key_cmp(current_pair->key, key) == 0) {
            current_pair->value = value;
            free(new_pair);
            return true;
        }
        current = current->next;
    }
    if (!new_node) {
        free(new_pair);
        return false;
    }
    new_node->value = new_pair;
    new_node->next = *map_ptr;
    *map_ptr = new_node;
    return true;
}

void *map_get_elem(map_t *map, void *key, key_comparator_t key_cmp)
{
    map_t *current = map;
    pair_t *pair = (pair_t *)current->value;

    if (!map || !key_cmp)
        return NULL;
    while (current) {
        if (key_cmp(pair->key, key) == 0)
            return pair->value;
        current = current->next;
    }
    return NULL;
}

bool map_del_elem(map_t **map_ptr, void *key, key_comparator_t key_cmp)
{
    list_t *current = *map_ptr, *previous = NULL;
    pair_t *pair = (pair_t *)current->value;

    if (!map_ptr || !*map_ptr || !key_cmp)
        return false;
    while (current) {
        if (key_cmp(pair->key, key) == 0) {
            *(previous ? &previous->next : map_ptr) = current->next;
            free(pair);
            free(current);
            return true;
        }
        previous = current, current = current->next;
    }
    return false;
}



int int_comparator ( const void *first , const void * second )
{
int val1 = *( const int *) first ;
int val2 = *( const int *) second ;
return ( val1 - val2 ) ;
}
int main ( void )
{
map_t * map = NULL ;
int first_key = 1;
int second_key = 2;
int third_key = 3;
char * first_value = " first ";
char * first_value_rw = " first_rw ";
char * second_value = " second ";
char * third_value = " third ";
char ** data = NULL ;
map_add_elem (& map , & first_key , & first_value , & int_comparator ) ;
map_add_elem (& map , & first_key , & first_value_rw , & int_comparator ) ;
map_add_elem (& map , & second_key , & second_value , & int_comparator ) ;
map_add_elem (& map , & third_key , & third_value , & int_comparator ) ;
data = ( char **) map_get_elem (map , & second_key , & int_comparator ) ;
printf (" The key [%d] maps to value [%s]\n", second_key , * data ) ;
map_clear (& map) ;
return (0) ;
}