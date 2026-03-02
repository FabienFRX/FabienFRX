/*
** EPITECH PROJECT, 2023
** info_file
** File description:
** project ls step by step
*/

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

int info_files(const char *fname)
{
    DIR* dir = opendir(fname);
    
    if (dir == NULL) {
        printf("File not found");
        return (84);
    }
    struct dirent* format;
    format = readdir(dir);
    while (format != NULL) {
        printf("%d, %s\n", format -> d_type, format -> d_name);
        if (format -> d_type == DT_DIR && strcmp(format -> d_name, ".") != 0 && strcmp(format -> d_name, "..") != 0) {
            char path[100] = {0};
            strcat(path, fname);
            strcat(path, "/");
            strcat(path, format -> d_name);
            info_files(path);
        }
        format = readdir(dir);
    }
    closedir(dir);
    return (0);
}

int main()
{
    info_files(".");
    info_files("..");
    return (0);
}
