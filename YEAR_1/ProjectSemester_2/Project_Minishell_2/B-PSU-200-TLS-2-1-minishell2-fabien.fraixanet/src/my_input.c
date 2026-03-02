/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_input
*/

#include "my_lib_incl.h"

static void update_counters(char ch, int *nb, int *count)
{
    if (ch == '|') {
        (*nb)++;
    } else if (ch == '>') {
        (*count)++;
    }
}

static int check_initial_conditions(char *cmd, int *nb, int *count)
{
    for (int i = 0; cmd[i]; i++) {
        update_counters(cmd[i], nb, count);
        if (*count > 0 && *nb != 0) {
            my_putstr("Ambiguous output redirect.\n", 2);
            return -2;
        }
        if (cmd[i] != ' ' && cmd[i] != '|' && cmd[i] != '\0') {
            *nb = 0;
        }
        if (*nb == 2) {
            return -1;
        }
    }
    return 0;
}

static int check_pipe_and_redirect(char *cmd)
{
    int nb = 0;
    int count = 0;

    if (check_initial_conditions(cmd, &nb, &count) != 0) {
        return -2;
    }
    if (nb != 0) {
        return -1;
    }
    return 0;
}

static int check_error_pipe(char *cmd)
{
    if (cmd == NULL || cmd[0] == '|')
        return -1;
    return check_pipe_and_redirect(cmd);
}

void process_commands(char **cmd, char **env)
{
    int nb = 0;

    for (int i = 0; cmd[i]; i++) {
        nb = check_error_pipe(cmd[i]);
        if (nb == -1) {
            my_putstr("Invalid null command.\n", 2);
        } else {
            (cmd[i][0] != '\0' && nb != -2);
            my_pipe(cmd[i], env);
        }
    }
}

void handle_prompt(void)
{
    if (isatty(0)) {
        my_putstr("> ", 1);
    }
}

int read_input(char **temp, size_t *len)
{
    return getline(temp, len, stdin);
}

void my_input(char **env)
{
    char *temp = NULL;
    char **cmd = NULL;
    size_t len = 0;

    single_env(env);
    while (1) {
        env = single_env(NULL);
        handle_prompt();
        if (read_input(&temp, &len) == -1) {
            free(temp);
            exit(0);
        }
        replace_char(temp, '\n', '\0');
        cmd = my_strtab(temp, ';');
        process_commands(cmd, env);
        free(temp);
        free_string_array(cmd);
    }
}
