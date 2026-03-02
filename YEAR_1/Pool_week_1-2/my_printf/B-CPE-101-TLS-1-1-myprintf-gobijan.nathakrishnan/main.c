/*
** EPITECH PROJECT, 2023
** main
** File description:
** test main printf
*/
#include <stdio.h>
#include <stdarg.h>
#include "my.h"

void main()
{
    float nb = 2598.85755;
    int var = 10;
    int *ptr = &var;
    unsigned int b = -1;
    
//    printf("%p\n", ptr);
//    print_p((int *)ptr);

//    my_printf("Number of characters written : %d\n", num);
//    printf("Number of characters written : %d\n", num);
    
    my_printf("hello %s, today is %d, in hex %x, %f, %p, %u\n", "john", 10, 365, nb, ptr, b);
    printf("hello %s, today is %d, in hex %x, %f, %p, %u\n", "john", 10, 365, nb, ptr, b);
//    my_printf("%p\n", ptr);
//    printf("%p\n",ptr);
//    length_n(str);
}
