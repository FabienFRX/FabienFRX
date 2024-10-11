/*
** EPITECH PROJECT, 2023
** fabien
** File description:
** add element to inventory
*/

#include <stdlib.h>
#include "include/my.h"

int args_length(char **args)
{
    int i = 0;

    for (; args[i] != NULL; i++) {
    }
    return (i);
}

static int find_type_index(char *type_to_find)
{
    for (int i = 0; i < type_value_lenght; i++) {
        if (my_strcmp(type_value[i], type_to_find) == 0) {
            return (i);
        }
    }
    return (-1);
}

/*
** param:
**  - type: string with the type given by user
**  - name: string with the name given by user
** return:
**   a new list with a unique element and no next element.
**   In case of error, return NULL;
*/
void *init_inventory(char *type, char *name)
{
    linked_list_t *list = malloc(sizeof(*list));
    object_t *object = malloc(sizeof(*object));
    type_t idx = find_type_index(type);

    if (list == NULL || object == NULL || idx == -1) {
        return (NULL);
    }
    object->id = 0;
    object->name = my_strdup(name);
    object->type = idx;
    list->data = object;
    list->next = NULL;
    return (list);
}

/*
** param:
**  - data: the current list
**  - type: string with the type given by user
**  - name: string with the name given by user
** return:
**   a new list with element given by user at the
**   head and pointing on the list given in parameter.
**   In case of error, return the current list
*/
linked_list_t *add_item(linked_list_t *inventory, char *type, char *name)
{
    linked_list_t *list = malloc(sizeof(*list));
    object_t *object = malloc(sizeof(*object));
    type_t idx = find_type_index(type);

    if (list == NULL || object == NULL || idx == -1) {
        return (inventory);
    }
    object->id = inventory->data->id + 1;
    object->name = my_strdup(name);
    object->type = idx;
    list->data = object;
    list->next = inventory;
    return (list);
}

int add(void *data, char **args)
{
    linked_list_t *inventory = NULL;

    if (data == NULL)
        return (84);
    inventory = *((linked_list_t **)data);
    if (args_length(args) % 2 == 1)
        return (84);
    for (int i = 0; args[i] != NULL; i += 2) {
        if (inventory == NULL) {
            inventory = init_inventory(args[i], args[i + 1]);
        } else {
            inventory = add_item(inventory, args[i], args[i + 1]);
        }
        if (inventory != NULL)
            my_printf("%s n°%d - \"%s\" added.\n",
                args[i], inventory->data->id, args[i + 1]);
    }
    *((linked_list_t **)data) = inventory;
    return (0);
}
