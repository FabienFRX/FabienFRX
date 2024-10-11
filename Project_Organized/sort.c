/*
** EPITECH PROJECT, 2023
** fabien
** File description:
** sort inventory
*/

#include <stdlib.h>
#include <stdio.h>
#include "include/my.h"


static int compare_type(object_t *ob1, object_t *ob2)
{
    if (ob1->type == ob2->type)
        return (0);
    if (ob1->type < ob2->type)
        return (-1);
    if (ob1->type > ob2->type)
        return (1);
}

static int compare_id(object_t *ob1, object_t *ob2)
{
    if (ob1->id == ob2->id)
        return (0);
    if (ob1->id < ob2->id)
        return (-1);
    if (ob1->id > ob2->id)
        return (1);
}

//renvoie possible = 0 si '=', -1 si ob1 < ob2, 1 si ob1 > ob2
static int compare(object_t *ob1, object_t *ob2,
    object_field_t field, int is_increas)
{
    object_t *tmp = NULL;

    if (is_increas == 0) {
        tmp = ob1;
        ob1 = ob2;
        ob2 = tmp;
    }
    if (field == ID) {
        return compare_id(ob1, ob2);
    } else if (field == NAME) {
        return (my_strcmp(ob1->name, ob2->name));
    } else {
        return compare_type(ob1, ob2);
    }
}

static linked_list_t *compare_to_others(linked_list_t *it, int key, int order)
{
    linked_list_t *min_pos = it;

    for (linked_list_t *it2 = it->next; it2 != NULL; it2 = it2->next) {
        if (compare(it2->data, min_pos->data, key, order) < 0) {
            min_pos = it2;
        }
    }
    return min_pos;
}

static int algo_sort(void *data, object_field_t key, int is_increas)
{
    linked_list_t *inventory = NULL;
    int res = 0;
    object_t *tmp = 0;
    linked_list_t *min_pos = NULL;

    if (data == NULL)
        return (84);
    inventory = *((linked_list_t **)data);
    if (inventory == NULL)
        return (0);
    for (linked_list_t *it = inventory; it->next != NULL; it = it->next) {
        min_pos = compare_to_others(it, key, is_increas);
        if (min_pos != it) {
            tmp = it->data;
            it->data = min_pos->data;
            min_pos->data = tmp;
        }
    }
    *((linked_list_t **)data) = inventory;
    return (0);
}

static int find_object_key(char *to_find)
{
    for (int j = 0; j < object_field_value_length; j++) {
        if (my_strcmp(object_field_value[j], to_find) == 0) {
            return (j);
        }
    }
    return (-1);
}

int sort(void *data, char **args)
{
    int key = -1;
    int is_increas = 1;

    for (int i = 0; args[i] != NULL; i++) {
        if (args[i][0] != '-' && key == -1)
            key = find_object_key(args[i]);
        if (args[i][0] == '-' && args[i][1] == 'r')
            is_increas = 0;
    }
    if (key == -1)
        return (84);
    algo_sort(data, key, is_increas);
    return (0);
}
