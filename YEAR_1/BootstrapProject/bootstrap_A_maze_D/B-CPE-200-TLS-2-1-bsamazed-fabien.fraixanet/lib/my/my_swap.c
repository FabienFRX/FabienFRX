/*
** EPITECH PROJECT, 2023
** my_swap
** File description:
** swaps the content of two integers in parameters
*/

void my_swap(int *a, int *b)
{
    int tmp = 0;

    tmp = *a;
    *a = *b;
    *b = tmp;
    return;
}
