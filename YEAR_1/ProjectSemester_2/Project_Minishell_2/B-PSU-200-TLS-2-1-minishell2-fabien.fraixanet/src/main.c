/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** main
*/

#include "my_lib_incl.h"

char *get_path(void)
{
    char *path = my_calloc_str(4096);

    if (path == NULL)
        return NULL;
    if (getcwd(path, 4096) == NULL) {
        free(path);
        return NULL;
    }
    return path;
}

char **initialize_env(char **env)
{
    char **new_env = my_tabdup(env);

    return new_env;
}

void initialize_pwd_array(char **pwd, char *pwd_value)
{
    pwd[0] = "setenv";
    pwd[1] = "PWD";
    pwd[2] = pwd_value;
    pwd[3] = NULL;
}

int main(int argc, char **argv, char **env)
{
    char *pwd_value;
    char *pwd[4];

    (void) argc;
    (void) argv;
    pwd_value = get_path();
    if (pwd_value == NULL)
        return EXIT_FAILURE;
    initialize_pwd_array(pwd, pwd_value);
    env = initialize_env(env);
    if (env == NULL) {
        free(pwd_value);
        return EXIT_FAILURE;
    }
    env = my_setenv(pwd, env);
    single_old(my_calloc_str(1));
    my_input(env);
    free(pwd_value);
    return EXIT_SUCCESS;
}
