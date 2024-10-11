/*
** EPITECH PROJECT, 2023
** main
** File description:
** main function
*/

#include <stdlib.h>
#include "include/my.h"
#include "include/shell.h"

int main(void)
{
    linked_list_t *inventory = NULL;
    int res = 0;

    res = workshop_shell(&inventory);
    if (res != 0) {
        return (84);
    }
    return (0);
}
