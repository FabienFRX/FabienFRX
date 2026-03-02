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

typedef struct env_s {
    char **curr_env;
} env_t;

void my_setenv(char **argv, env_t *envp, char **env);
void parse_env(char **argv, env_t *envp);
void my_env(env_t *envp);
bool my_strcmp_to_char(char *s1, char *s2, char c);
void my_cd(char **argv, char **env);
void my_ls(char **argv, char **env);
#endif /*MY_SH_H*/
