/*
** EPITECH PROJECT, 2023
** fabien
** File description:
** delete element from inventory
*/

#include <stdlib.h>
#include "include/my.h"

static linked_list_t *remove_head(linked_list_t *list)
{
    linked_list_t *tmp = NULL;

    my_printf("%s n°%d - \"%s\" deleted.\n",
        type_value[list->data->type], list->data->id, list->data->name);
    tmp = list->next;
    free(list->data->name);
    free(list->data);
    if (tmp != NULL)
        free(list);
    return (tmp);
}

static void remove_link(linked_list_t *item)
{
    linked_list_t *tmp = NULL;

    my_printf("%s n°%d - \"%s\" deleted.\n",
        type_value[item->next->data->type], item->next->data->id,
        item->next->data->name);
    tmp = item->next->next;
    free(item->next->data->name);
    free(item->next->data);
    free(item->next);
    item->next = tmp;
}

static linked_list_t *find_index(linked_list_t *inventory, int id)
{
    if (inventory == NULL)
        return NULL;
    for (linked_list_t *it = inventory; it != NULL; it = it->next) {
        if (it->data->id == id) {
            inventory = remove_head(inventory);
            break;
        }
        if (it->next != NULL && it->next->data->id == id) {
            remove_link(it);
            break;
        }
    }
    return inventory;
}

int del(void *data, char **args)
{
    linked_list_t *inventory = NULL;
    int id = -1;

    if (data == NULL)
        return (84);
    inventory = *((linked_list_t **)data);
    for (int i = 0; args[i] != NULL; i++) {
        id = my_getnbr(args[i]);
        inventory = find_index(inventory, id);
    }
    *((linked_list_t **)data) = inventory;
    return (0);
}
