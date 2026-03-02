/*
** EPITECH PROJECT, 2024
** file_reader
** File description:
** azerty
*/

#include "../include/my_navy.h"

char **openfile(char *filepath)
{
    char **pos;
    char *buff = malloc(sizeof(char) * 32 + 1);
    int fd = open(filepath, O_RDONLY);
    int reading = read(fd, buff, 32);

    if (fd == -1 || reading < 0)
        return NULL;
    if (boat_counter(buff) == 84) {
        free(buff);
        return NULL;
    }
    buff = clean_buffer(buff);
    pos = split_pos(buff);
    return pos;
}

char **split_pos(char *buff)
{
    char **pos = malloc(sizeof(char *) * 4);
    int j = 0; 
    int z = 0;

    for (int i = 0; i < 4; i++)
        pos[i] = malloc(sizeof(char) * (32 + 1));
    for (int i = 0; buff[i] != '\0' && j < 4; i++, z++) {
        if (buff[i] == '\n') {
            i++;
            pos[j][z] = '\0';
            j++;
            z = -1;
        } else {
            pos[j][z] = buff[i];
        }
    }
    return pos;
}

int boat_counter(char *buff)
{
    int nb = 0;

    for (int i = 0; buff[i] != '\0'; i++)
        if (buff[i] == '\n')
            nb++;
    if (nb != 4) {
        return 84;
    }
    return 0;
}

char *clean_buffer(char *buff)
{
    int nb_boat = 4;
    int counter = 0;

    for (int i = 0; buff[i] != '\0'; i++) {
        if (buff[i] == '\n')
            counter++;
        if (counter == 4) {
            buff[i + 1] = '\0';
            break;
        }     
    }
    return buff;
}
