/*
** EPITECH PROJECT, 2024
** File reader
** File description:
** qwerty
*/

#include "../include/asm.h"

static off_t file_size(const char *filepath)
{
    struct stat file_stat;

    if (stat(filepath, &file_stat) == -1) {
    return -84;
    }
    return file_stat.st_size;
}

static int read_file(int fd, char *buff, off_t size)
{
    int reading = read(fd, buff, size);

    if (reading == -1) {
        free(buff);
        close(fd);
        return -1;
    }
    return reading;
}

char *file_reader(char const *filepath)
{
    char *buff;
    int fd = open(filepath, O_RDONLY);
    off_t size = file_size(filepath);

    if (fd == -1) {
        my_printf("Error opening file.\n");
        return NULL;
    }
    buff = malloc(sizeof(char) * (size + 1));
    if (buff == NULL) {
        close(fd);
        return NULL;
    }
    read_file(fd, buff, size);
    close(fd);
    buff[size] = '\0';
    return buff;
}
