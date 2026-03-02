/*
** EPITECH PROJECT, 2023
** fyle_sys
** File description:
** all function for file
*/

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "include/my.h"

int fs_open_file(char const *filepath)
{
    int fd = open(filepath, O_RDONLY);
    
    close(fd);
    return (0);
}