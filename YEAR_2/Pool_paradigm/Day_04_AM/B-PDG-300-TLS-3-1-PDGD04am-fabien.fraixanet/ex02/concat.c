/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD04am-fabien.fraixanet
** File description:
** concat
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "concat.h"

void concat_strings(const char *str1, const char *str2, char **res)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t total_len = len1 + len2 + 1;

    if (!res)
        return;
    if (!str1)
        str1 = "";
    if (!str2)
        str2 = "";
    *res = malloc(total_len);
    if (!*res) {
        perror("malloc failed");
        return;
    }
    strcpy(*res, str1);
    strcat(*res, str2);
}

void concat_struct(concat_t *str)
{
    if (!str)
        return;
    concat_strings(str->str1, str->str2, &str->res);
}
