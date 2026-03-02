/*
** EPITECH PROJECT, 2024
** B-CPE-200-TLS-2-1-bsrobotfactory-fabien.fraixanet
** File description:
** step3
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/my.h"

int main(void)
{
    int a = 12345678;
    FILE *fp = fopen("./number-as-int.yolo", "w+");

    putw(a, fp);
    fclose(fp);
    return (0);
}
