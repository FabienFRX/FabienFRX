/*
** EPITECH PROJECT, 2024
** B-CPE-200-TLS-2-1-bsamazed-fabien.fraixanet
** File description:
** step_2
*/

#include "../include/my.h"

void print_link(link_t *link)
{
    link_t *tmp = link;

    while (tmp != NULL) {
        printf("%d\n", tmp->data);
        tmp = tmp->next;
    }
}
