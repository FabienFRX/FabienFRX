/*
** EPITECH PROJECT, 2023
** my_ls.h
** File description:
** all include of function
*/

#ifndef MY_SH_H
    #define MY_SH_H
    #include <stdlib.h>
    #include <stdio.h>
    #include <stdbool.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <dirent.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <sys/wait.h>
    #include <signal.h>
    #include "my.h"

void my_setenv(char **argv, char **env);
void parse_env(char **argv, char **env);
void my_env(char **env);
bool my_strcmp_to_char(char *s1, char *s2, char c);
void my_cd(char **argv, char **env);
void my_ls(char **argv, char **env);
const char *get_env_var(const char *name, char **env);
#endif /*MY_SH_H*/
