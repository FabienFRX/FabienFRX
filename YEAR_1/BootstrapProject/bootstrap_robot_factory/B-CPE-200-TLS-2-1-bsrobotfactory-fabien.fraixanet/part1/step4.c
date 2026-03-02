/*
** EPITECH PROJECT, 2024
** B-CPE-200-TLS-2-1-bsrobotfactory-fabien.fraixanet
** File description:
** step4
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/my.h"

int main(void)
{
    int a = 192837;
    char c = 'k';
    char str[40] = "Corewar is swag!!";
    FILE *fp = fopen("./several-variables.yolo", "w+");

    putw(a, fp);
    fprintf(fp, "%d\n", a);
    fprintf(fp, "%c\n", c);
    fprintf(fp, "%s\n", str);
    fclose(fp);
    return (0);
}
