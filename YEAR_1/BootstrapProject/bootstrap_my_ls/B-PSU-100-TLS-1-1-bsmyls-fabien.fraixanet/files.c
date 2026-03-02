/*
** EPITECH PROJECT, 2023
** files
** File description:
** function for stat
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv)
{
    struct stat f;
    printf("file : \n");
    switch (f.st_mode & S_IFMT) {
    case S_IFDIR:
        printf("repertoire\n");
        break;
    }
    printf("Taille du fichier : %lld octets\n", f.st_size);
    return (0);
}
