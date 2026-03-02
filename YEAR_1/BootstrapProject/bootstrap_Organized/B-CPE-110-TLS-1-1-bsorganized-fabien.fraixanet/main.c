/*
** EPITECH PROJECT, 2023
** main
** File description:
** main function
*/

#include "include/my.h"
#include "include/organized.h"
#include "include/bootstrap.h"

int main(void)
{
    int nb = 3;
    char c = 'k';
    char *str = my_strdup("Fabien");
    player_t player = {my_strdup("Nau"), 98};

    print_typed_value(&nb, INTEGER);
    print_typed_value(&c, CHAR);
    print_typed_value(&str, STRING);
    print_typed_value(&player, PLAYER);
    return (0);
}
