/*
** EPITECH PROJECT, 2024
** B-CPE-200-TLS-2-1-bsamazed-fabien.fraixanet
** File description:
** step_3
*/

#include "../include/my.h"

void connect_links(link_t *link1, link_t *link2)
{
    link1->next = link2;
    link2->next = link1;
}
