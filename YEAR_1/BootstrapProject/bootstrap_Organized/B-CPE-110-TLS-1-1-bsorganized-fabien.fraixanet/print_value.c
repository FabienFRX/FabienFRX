/*
** EPITECH PROJECT, 2023
** print_value
** File description:
** function that handle void pointers
*/

#include "include/my.h"
#include "include/organized.h"
#include "include/bootstrap.h"

int print_typed_value(void *data, type_t type)
{
    if (type = INTEGER) {
        my_printf("%d\n", *(int *)data);
    }
    if (type = CHAR) {
        my_printf("%c\n", *(char *)data);
    }
    if (type = STRING) {
        my_printf("%s\n", data);
    }
    /*if (type = PLAYER) {
        my_printf("");
    }*/
    if (&data == NULL) {
        return (84);
    }
    if (type == NULL) {
        return (84);
    }

    return (0);
}
