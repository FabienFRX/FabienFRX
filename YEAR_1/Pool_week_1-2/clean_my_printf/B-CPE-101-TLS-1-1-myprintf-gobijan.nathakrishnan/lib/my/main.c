/*
** EPITECH PROJECT, 2023
** main
** File description:
** main test
*/
#include <stdio.h>

int main(void)
{
    char *str = "jhon";
    float f = 12.45;
    int loc = 16;
    int *ptr = &loc;
    unsigned int nb = -12;
    double en = 11357.213548;
    double ne = -112354.458796;

//    printf("%g\n", g);
//    my_print_g_low("g\n");
    
    printf("hello, %d, %E, %b, %X, %e, %s, %f, %p, %x, %u\n", 47, ne, -123, -365, en, str, f, ptr, -134, nb);
    my_printf("hello %d, %E, %b, %X, %e, %s, %f, %p, %x, %u\n", 47, ne, -123, -365, en, str, f, ptr, -134, nb);
//    printf("%li, %lu, %lx\n", 1234567, -1234567, 12364879);
//    my_printf2("%li, %lu, %lx\n", 1234567, -1234567, 12364879);
//    printf("%X, %x\n", -365, -365);
//    my_printf("%X, %x\n", -365, -365);
//    printf("hello %x, %X\n", 365, 365);
//    my_printf("hello %x, %X\n", 365, 365);
//    printf("%e\n", 142248.132547);
//    printf("%e\n", 32.120000);
//    printf("%e\n", 000.001275);
//    printf("test%\n");
//    my_printf("test%\n");
    return (0);
}
