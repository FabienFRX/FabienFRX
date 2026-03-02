/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD05-fabien.fraixanet
** File description:
** clear
*/

#include "string.h"
#include <stdlib.h>
#include <string.h>

void clear(string_t *this)
{
    if (this == NULL || this->str == NULL)
        return;
    free(this->str);
    this->str = strdup("");
}
