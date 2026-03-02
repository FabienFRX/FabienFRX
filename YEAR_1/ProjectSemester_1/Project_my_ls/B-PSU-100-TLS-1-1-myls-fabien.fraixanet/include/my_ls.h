/*
** EPITECH PROJECT, 2023
** my_ls.h
** File description:
** all include of function
*/

#ifndef MY_LS_H
    #define MY_LS_H
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <sys/time.h>
    #include <dirent.h>
    #include <pwd.h>
    #include <grp.h>
    #include <time.h>
    #include <fcntl.h>
    #include "my.h"

    #define PATH 4096

int list_options(const char *path, int recurs, int hide, int long_form);
int different_option(char option, int *hide, int *long_form, int *recurs);
void permission(mode_t mode);
void print_info(const char *all_path, const struct stat *all_info);
void file_process(const char *path, const struct dirent *entity,
    int long_form);
void full_process(const char *path, const char *file, char *all_path);
void process_dir(const char *path, int recurs, int hide, int long_form);
#endif /*MY_LS_H*/
