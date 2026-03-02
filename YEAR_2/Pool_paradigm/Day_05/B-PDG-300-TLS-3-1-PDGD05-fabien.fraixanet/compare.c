/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD05-fabien.fraixanet
** File description:
** compare
*/

#include "string.h"
#include <stdlib.h>
#include <string.h>

int compare_s(const string_t *this, const string_t *str)
{
    if (this == NULL || this->str == NULL || str == NULL || str->str == NULL) {
        return -1;
    }
    return strcmp(this->str, str->str);
}

int compare_c(const string_t *this, const char *str)
{
    if (this == NULL || this->str == NULL || str == NULL) {
        return -1;
    }
    return strcmp(this->str, str);
}
