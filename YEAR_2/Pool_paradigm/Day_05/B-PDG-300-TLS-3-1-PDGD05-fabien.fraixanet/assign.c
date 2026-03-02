/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD05-fabien.fraixanet
** File description:
** assign
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "string.h"

void assign_s(string_t *this, const string_t *str)
{
    if (this == NULL || str == NULL || str->str == NULL)
        return;
    if (this->str != NULL)
        free(this->str);
    this->str = strdup(str->str);
}

void assign_c(string_t *this, const char *s)
{
    if (this == NULL || s == NULL)
        return;
    if (this->str != NULL)
        free(this->str);
    this->str = strdup(s);
}
