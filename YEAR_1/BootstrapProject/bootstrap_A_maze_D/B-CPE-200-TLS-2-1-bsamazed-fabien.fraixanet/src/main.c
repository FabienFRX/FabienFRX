/*
** EPITECH PROJECT, 2024
** B-CPE-200-TLS-2-1-bsamazed-fabien.fraixanet
** File description:
** main
*/

#include "../include/my.h"

int main(int argc, char **argv)
{
    link_t *node = create_link(42);
    print_link(node);
    return(0);
}