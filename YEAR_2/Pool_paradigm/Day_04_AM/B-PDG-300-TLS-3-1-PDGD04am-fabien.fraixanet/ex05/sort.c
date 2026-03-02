/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD04am-fabien.fraixanet
** File description:
** sort
*/

#include <stdlib.h>
#include <string.h>

void sort_array(void *arr, size_t nmemb, size_t size,
    int (*compar)(const void *, const void *))
{
    char *temp = NULL;

    for (size_t i = 0; i < nmemb - 1; i++) {
        for (size_t j = i + 1; j < nmemb; j++) {
            if (compar((char *)arr + i * size, (char *)arr + j * size) > 0) {
                temp[size];
                memcpy(temp, (char *)arr + i * size, size);
                memcpy((char *)arr + i * size, (char *)arr + j * size, size);
                memcpy((char *)arr + j * size, temp, size);
            }
        }
    }
}

void sort_int_array(int *array, size_t nmemb)
{
    int temp = 0;

    for (size_t i = 0; i < nmemb - 1; i++) {
        for (size_t j = i + 1; j < nmemb; j++) {
            if (array[i] > array[j]) {
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
}
