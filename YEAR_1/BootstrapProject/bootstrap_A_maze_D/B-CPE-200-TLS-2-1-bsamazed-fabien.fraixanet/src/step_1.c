/*
** EPITECH PROJECT, 2024
** B-CPE-200-TLS-2-1-bsamazed-fabien.fraixanet
** File description:
** display_link
*/

#include "../include/my.h"

link_t *create_link(int data)
{
	link_t *node = malloc(sizeof(link_t));

	node->data = data;
	node->next = NULL;
	return (node);
}
