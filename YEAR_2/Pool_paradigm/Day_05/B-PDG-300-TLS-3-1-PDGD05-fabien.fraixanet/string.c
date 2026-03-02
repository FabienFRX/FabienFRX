/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD05-fabien.fraixanet
** File description:
** string
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string.h"

void string_init2(string_t *this)
{
    this->copy = &copy;
    this->c_str = &c_str;
    this->empty = &empty;
    this->find_s = &find_s;
    this->find_c = &find_c;
    this->insert_c = &insert_c;
    this->insert_s = &insert_s;
    this->to_int = &to_int;
}

void string_init(string_t *this, const char *s)
{
    if (this == NULL)
        return;
    if (s) {
        this->str = strdup(s);
    } else {
        this->str = malloc(1);
        this->str[0] = '\0';
    }
    this->assign_s = &assign_s;
    this->assign_c = &assign_c;
    this->append_s = &append_s;
    this->append_c = &append_c;
    this->at = &at;
    this->clear = &clear;
    this->length = &length;
    this->compare_s = &compare_s;
    this->compare_c = &compare_c;
    string_init2(this);
}

void string_destroy(string_t *this)
{
    if (this == NULL) {
        return;
    }
    free(this->str);
    this->str = NULL;
}
