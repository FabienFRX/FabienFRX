/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD05-fabien.fraixanet
** File description:
** c_str
*/

#include "string.h"
#include <stdlib.h>
#include <string.h>

const char *c_str(const string_t *this)
{
    if (this == NULL || this->str == NULL) {
        return NULL;
    }
    return this->str;
}
