/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD05-fabien.fraixanet
** File description:
** empty
*/

#include "string.h"
#include <stdlib.h>
#include <string.h>

int empty(const string_t *this)
{
    if (this == NULL || this->str == NULL) {
        return 1;
    }
    if (strlen(this->str) == 0) {
        return 1;
    }
    return 0;
}
