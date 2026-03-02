/*
** EPITECH PROJECT, 2023
** sorting_algo
** File description:
** funtion that handle sorting function
*/

#include "include/my.h"
#include "include/organized.h"
#include "include/bootstrap.h"

void selection_sort(int *array, int len)
{
    int set_value;
    int set_new_value;
    int i = 0;
    char *idx;

    while (array[i] == set_value) {
        array[i]++; // iterate: set first value as minimum then iterate through the others elements
    if (set_value < array[i + 1]) {
        set_new_value;
        my_swap(set_new_value[idx], set_value[idx]);
    }       // check next elements if inferior if inferior set this new min 
            // swap the set and the min value
    }
    array[i] = array[i] + 1;    // do again till no more value (++)
}
