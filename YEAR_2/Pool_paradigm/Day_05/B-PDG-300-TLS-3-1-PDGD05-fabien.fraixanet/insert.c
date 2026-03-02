/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD05-fabien.fraixanet
** File description:
** insert
*/

#include "string.h"
#include <stdlib.h>
#include <string.h>

void insert_c(string_t *this, size_t pos, const char *str)
{
    size_t len_str = strlen(str);
    size_t len_this = strlen(this->str);
    char *new_str = malloc(len_this + len_str + 1);

    if (this == NULL || this->str == NULL || str == NULL) {
        return;
    }
    if (pos > len_this) {
        pos = len_this;
    }
    if (new_str == NULL) {
        return;
    }
    strncpy(new_str, this->str, pos);
    strcpy(new_str + pos, str);
    strcpy(new_str + pos + len_str, this->str + pos);
    free(this->str);
    this->str = new_str;
}

void insert_s(string_t *this, size_t pos, const string_t *str)
{
    if (this == NULL || this->str == NULL || str == NULL || str->str == NULL) {
        return;
    }
    insert_c(this, pos, str->str);
}
