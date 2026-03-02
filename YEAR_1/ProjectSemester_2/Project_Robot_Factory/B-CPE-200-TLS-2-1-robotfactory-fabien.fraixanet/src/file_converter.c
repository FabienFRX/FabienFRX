/*
** EPITECH PROJECT, 2024
** B-CPE-200-TLS-2-1-robotfactory-fabien.fraixanet [WSL: Debian]
** File description:
** file_converter
*/

#include "../include/asm.h"

char *convert_in_cor(testa_t *head)
{
    char *cor = malloc(sizeof(char) * (my_strlen(head->name[0]) + 5));
    int fd = 0;

    my_strcpy(cor, head->name[0]);
    my_strcat(cor, ".cor");
    fd = open(cor, O_CREAT | O_WRONLY, 0666);
    if (fd == -1)
        return NULL;
    write(fd, MAGIC_HEADER, 4);
    write(fd, head->name[0], my_strlen(head->name[0]));
    write(fd, "\0\0\0\0", 4);
    write(fd, "\0\0\0\0", 4);
    write(fd, head->comment[0], my_strlen(head->comment[0]));
    write(fd, "\0\0\0\0", 4);
    write(fd, head->data, my_strlen(head->data));
    close(fd);
    return cor;
}
