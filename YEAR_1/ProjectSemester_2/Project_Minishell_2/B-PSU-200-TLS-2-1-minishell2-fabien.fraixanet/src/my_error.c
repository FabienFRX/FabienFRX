/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_error
*/

#include "my_lib_incl.h"

char *build_error_msg(char *cmd, char *str, int err)
{
    size_t len = strlen(cmd) + strlen(str) + 5;
    char *full_error_msg = malloc(len * sizeof(char));

    if (err == 8) {
        len += 20;
    }
    if (full_error_msg == NULL) {
        write_my_error("Impossible to malloc");
        return NULL;
    }
    strcpy(full_error_msg, cmd);
    strcat(full_error_msg, ": ");
    strcat(full_error_msg, str);
    if (err == 8) {
        strcat(full_error_msg, ". Wrong Architecture");
    }
    strcat(full_error_msg, ".\n");
    return full_error_msg;
}

void my_error(char *cmd, int err)
{
    char *str = strerror(err);
    char *full_error_msg = build_error_msg(cmd, str, err);

    my_putstr(full_error_msg, 2);
    free(full_error_msg);
}
