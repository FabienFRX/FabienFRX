/*
** EPITECH PROJECT, 2023
** fabien
** File description:
** display inventory
*/

#include <stdlib.h>
#include "include/my.h"

int disp(void *data, char **args)
{
    linked_list_t *inventory = NULL;
    linked_list_t *item = NULL;

    if (data == NULL) {
        return (84);
    }
    inventory = *((linked_list_t **)data);
    item = inventory;
    while (item != NULL) {
        my_printf("%s n°%d - \"%s\"\n", type_value[item->data->type],
            item->data->id, item->data->name);
        item = item->next;
    }
    return (0);
}
