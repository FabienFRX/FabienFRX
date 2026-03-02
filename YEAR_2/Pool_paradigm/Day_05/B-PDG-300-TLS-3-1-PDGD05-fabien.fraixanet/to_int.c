/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD05-fabien.fraixanet
** File description:
** to_int
*/

#include "string.h"
#include <stdlib.h>
#include <string.h>

int to_int(const string_t *this)
{
    if (this == NULL || this->str == NULL) {
        return 0;
    }
    return atoi(this->str);
}
