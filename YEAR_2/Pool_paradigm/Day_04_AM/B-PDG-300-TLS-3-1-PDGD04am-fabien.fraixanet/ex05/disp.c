/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD04am-fabien.fraixanet
** File description:
** disp
*/

#include <stdio.h>

void disp_array(const void *array, size_t nmemb, size_t size,
    void (*print)(const void *))
{
    for (size_t i = 0; i < nmemb; i++) {
        print((const char *)array + i * size);
    }
    printf("\n");
}

void disp_int_array(const int *array, size_t nmemb)
{
    for (size_t i = 0; i < nmemb; i++) {
        printf("%d\n", array[i]);
    }
}
