/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_builtin_cd
*/

#include "my_lib_incl.h"

char **handle_cd_other(char **env, char **cmd, char *pwd_now, char *pwd[4])
{
    if (chdir(cmd[1]) == -1) {
        my_error(cmd[1], errno);
    } else {
        pwd[2] = get_path();
        env = my_setenv(pwd, env);
        single_old(pwd_now);
    }
    return env;
}

char **handle_cd_dash(char **env, char *pwd_old, char *pwd_now, char *pwd[4])
{
    if (chdir(pwd_old) == -1) {
        my_error("", errno);
    } else {
        env = my_setenv(pwd, env);
        single_old(pwd_now);
    }
    return env;
}

static char **my_change_dir(char **cmd, char **env, char *pwd_now)
{
    char *pwd_old = single_old(NULL);
    char *pwd[4] = {"setenv", "PWD", pwd_old, NULL};

    if (my_strlen(cmd[1]) == 1 && cmd[1][0] == '-') {
        env = handle_cd_dash(env, pwd_old, pwd_now, pwd);
    } else {
        env = handle_cd_other(env, cmd, pwd_now, pwd);
    }
    return env;
}

char **handle_cd_no_args(char *home, char **env, char *pwd_now)
{
    char *pwd[4] = {"setenv", "PWD", home + 5, NULL};

    if (chdir(home + 5) == -1) {
        if (home == NULL)
            my_putstr("cd: No home directory.\n", 2);
        else
            my_putstr("cd: Can't change to home directory.\n", 2);
    } else {
        env = my_setenv(pwd, env);
        single_old(pwd_now);
    }
    return env;
}

char **my_cd(char **cmd, char **env)
{
    char *home = my_strdup(my_getenv(env, "HOME"));
    char *pwd_now = get_path();

    if (my_strlen_array(cmd) > 2) {
        my_putstr("cd: Too many arguments.\n", 2);
        return env;
    } else if (my_strlen_array(cmd) == 1) {
        env = handle_cd_no_args(home, env, pwd_now);
    } else {
        env = my_change_dir(cmd, env, pwd_now);
    }
    free(home);
    return env;
}
