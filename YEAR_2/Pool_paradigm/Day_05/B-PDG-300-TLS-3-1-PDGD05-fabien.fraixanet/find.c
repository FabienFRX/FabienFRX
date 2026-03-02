/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD05-fabien.fraixanet
** File description:
** find
*/

#include "string.h"
#include <stdlib.h>
#include <string.h>

int find_s(const string_t *this, const string_t *str, size_t pos)
{
    size_t len_this = strlen(this->str);
    size_t len_str = strlen(str->str);
    const char *found = strstr(this->str + pos, str->str);

    if (this == NULL || this->str == NULL || str == NULL || str->str == NULL) {
        return -1;
    }
    if (pos >= len_this || len_str == 0 || len_str > len_this) {
        return -1;
    }
    if (found == NULL) {
        return -1;
    }
    return found - this->str;
}

int find_c(const string_t *this, const char *str, size_t pos)
{
    size_t len_this = strlen(this->str);
    size_t len_str = strlen(str);
    const char *found = strstr(this->str + pos, str);

    if (this == NULL || this->str == NULL || str == NULL) {
        return -1;
    }
    if (pos >= len_this || len_str == 0 || len_str > len_this) {
        return -1;
    }
    if (found == NULL) {
        return -1;
    }
    return found - this->str;
}
