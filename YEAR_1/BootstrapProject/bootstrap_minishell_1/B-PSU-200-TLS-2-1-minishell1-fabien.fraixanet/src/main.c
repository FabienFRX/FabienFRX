/*
** EPITECH PROJECT, 2024
** B-PSU-200-TLS-2-1-minishell1-fabien.fraixanet
** File description:
** main
*/
#include "../include/my.h"

void seg_fault(int status)
{
    switch (status) {
        case 139:
        my_printf("segmentation fault (core dumped)");
        break;
        default:
        break;
    }
}

void comm_fork(char **argv, env_t *envp)
{
    pid_t p;
    int status;

    p = fork();
    if (p == 0) {
        parse_env(argv, envp);
        exit(0);
    }
    waitpid(p, &status, 0);
    seg_fault(status);
}

env_t *init_env(char **env)
{
    env_t *temp = malloc(sizeof(env_t));

    temp->curr_env = env;
    return (temp);
}

void all_commands(char *prompt, env_t *envp, char **env)
{
    char **argv = my_str_to_word_array(prompt, " \n");

    comm_fork(argv, envp);
    if (my_strcmp(argv[0], "env") == 0)
        my_env(envp);
    if (my_strcmp(argv[0], "cd") == 0)
        my_cd(argv, env);
    if (my_strcmp(argv[0], "setenv") == 0)
        my_setenv(argv, envp, env);
    if (my_strcmp(argv[0], "ls") == 0) {
        my_ls(argv, env);
    }
}

int main(int argc, char **argv, char **env)
{
    size_t line;
    char *prompt;
    env_t *envp = init_env(env);

    while (1) {
        my_putstr("$> ");
        if (getline(&prompt, &line, stdin) == -1)
            break;
        if (my_strcmp(prompt, "exit\n") == 0)
            break;
        all_commands(prompt, envp, env);
    }
    free(prompt);
    return (0);
}
