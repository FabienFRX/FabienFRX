/*
** EPITECH PROJECT, 2024
** B-CPE-200-TLS-2-1-bsrobotfactory-fabien.fraixanet
** File description:
** step5
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/my.h"

int main(void)
{
    struct disp_s{
        int a;
        char c;
        char str;
    }disp_t;

    FILE *fp = fopen("./one-structure.yolo", "w+");
    disp_t.a = 192837;
    disp_t.c = 'k';
    disp_t.str = [40];

    putw(a, fp);
    fprintf(fp, "%d\n", a);
    fprintf(fp, "%c\n", c);
    fprintf(fp, "%s\n", str);
    fclose(fp);
    return (0);
}