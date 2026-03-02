/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD05-fabien.fraixanet
** File description:
** at
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string.h"

char at(const string_t *this, size_t pos)
{
    if (this == NULL || this->str == NULL || strlen(this->str) <= pos)
        return (-1);
    return (this->str[pos]);
}
