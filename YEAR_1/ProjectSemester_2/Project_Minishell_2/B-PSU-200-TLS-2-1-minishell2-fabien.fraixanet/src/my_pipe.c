/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_pipe
*/

#include "my_lib_incl.h"

static int double_redir(char *cmd)
{
    static int status = 0;
    int i = 0;
    int nb = 0;

    if (cmd == NULL) {
        return status;
    }
    while (cmd[i]) {
        if (cmd[i] == '>') {
            nb++;
        }
        if (nb == 2) {
            status = 1;
        }
        i++;
    }
    return status;
}

static int open_file(char *file)
{
    if (double_redir(NULL) == 0) {
        return open(clean_string(file), O_RDWR | O_CREAT | O_TRUNC, 0666);
    } else {
        return open(clean_string(file), O_RDWR | O_CREAT | O_APPEND, 0666);
    }
}

static void last_exec_cmd(char **cmd, char **temp, int fd, char **env)
{
    int fl = 0;

    dup2(fd, 0);
    if (temp[1] != NULL) {
        fl = open_file(temp[1]);
        dup2(fl, 1);
        close(fl);
    }
    if (is_builtin(cmd[0]) != 1) {
        my_treat_cmd(cmd, env);
    } else {
        exit(0);
    }
}

static int my_dup_cmd(char **cmd, char **env, int fd)
{
    int tb[2] = {-1, -1};
    int pid = 0;

    pipe(tb);
    pid = fork();
    if (pid == 0) {
        close(tb[0]);
        dup2(fd, 0);
        close(fd);
        dup2(tb[1], 1);
        close(tb[1]);
        my_treat_cmd(cmd, env);
        if (is_builtin(cmd[0]) == 1) {
            exit(0);
        }
    }
    close(fd);
    close(tb[1]);
    free_string_array(cmd);
    return tb[0];
}

void handle_pipes(char **cmd, char **env, int *fd)
{
    for (int i = 0; cmd[i + 1]; i++) {
        *fd = my_dup_cmd(my_strtab(cmd[i], ' '), env, *fd);
    }
}

void handle_parent_process(int pid, int *status, char **cmd, char **env)
{
    waitpid(pid, status, F_OK);
    check_signal(*status);
    if (is_builtin(cmd[0]) == 1) {
        my_treat_cmd(cmd, env);
    }
}

void my_pipe(char *str, char **env)
{
    int fd = dup(0);
    char **cmd = my_strtab(str, '|');
    char **temp = NULL;
    int status = 0;
    int pid = 0;

    handle_pipes(cmd, env, &fd);
    double_redir(cmd[my_strlen_array(cmd) - 1]);
    temp = my_strtab(cmd[my_strlen_array(cmd) - 1], '>');
    cmd = my_strtab(temp[0], ' ');
    pid = fork();
    if (pid == 0) {
        last_exec_cmd(cmd, temp, fd, env);
    } else {
        handle_parent_process(pid, &status, cmd, env);
    }
}
