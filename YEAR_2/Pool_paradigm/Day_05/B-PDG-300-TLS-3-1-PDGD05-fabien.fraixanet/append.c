/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD05-fabien.fraixanet
** File description:
** append
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string.h"

void append_s(string_t *this, const string_t *ap)
{
    size_t this_len = (this->str != NULL) ? strlen(this->str) : 0;
    size_t ap_len = strlen(ap->str);
    size_t new_size = this_len + ap_len + 1;

    if (this == NULL || ap == NULL || ap->str == NULL)
        return;
    this->str = realloc(this->str, new_size);
    strcat(this->str, ap->str);
}

void append_c(string_t *this, const char *ap)
{
    size_t this_len = (this->str != NULL) ? strlen(this->str) : 0;
    size_t ap_len = strlen(ap);
    size_t new_size = this_len + ap_len + 1;

    if (this == NULL || ap == NULL)
        return;
    this->str = realloc(this->str, new_size);
    strcat(this->str, ap);
}
