/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD04am-fabien.fraixanet
** File description:
** uniq
*/

#include <stdlib.h>
#include <string.h>

size_t uniq_array(void *array, size_t nmemb, size_t size,
    int (*compar)(const void *, const void *))
{
    if (nmemb == 0) return 0;

    size_t unique_count = 1;
    for (size_t i = 1; i < nmemb; i++) {
        int is_duplicate = 0;
        for (size_t j = 0; j < unique_count; j++) {
            if (compar((char *)array + i * size, (char *)array + j * size) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        if (!is_duplicate) {
            memcpy((char *)array + unique_count * size, (char *)array + i * size, size);
            unique_count++;
        }
    }
    return unique_count;
}

size_t uniq_int_array(int *array, size_t nmemb)
{
    if (nmemb == 0) return 0;
    int is_duplicate = 0;
    size_t unique_count = 1;

    for (size_t i = 1; i < nmemb; i++) {
        for (size_t j = 0; j < unique_count; j++) {
            if (array[i] == array[j]) {
                is_duplicate = 1;
                break;
            }
        }
        if (!is_duplicate) {
            array[unique_count++] = array[i];
        }
    }
    return unique_count;
}
