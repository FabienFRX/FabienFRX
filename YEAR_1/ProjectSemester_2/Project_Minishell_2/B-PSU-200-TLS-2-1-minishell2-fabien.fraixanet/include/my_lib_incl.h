/*
** EPITECH PROJECT, 2024
** Minishell-2
** File description:
** my_lib_incl.h
*/

#ifndef MY_LIB_INCL
    #define MY_LIB_INCL

    #include <unistd.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <fcntl.h>
    #include <linux/limits.h>
    #include <sys/wait.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <errno.h>
    #include <string.h>

char *get_path(void);
char **initialize_env(char **env);
void initialize_pwd_array(char **pwd, char *pwd_value);

void free_strtab(char **tab);
int check_path_access(char **temp, char *str);
int my_access(char *str, char *path);

char **handle_cd_other(char **env, char **cmd, char *pwd_now, char *pwd[4]);
char **handle_cd_dash(char **env, char *pwd_old, char *pwd_now, char *pwd[4]);
char **handle_cd_no_args(char *home, char **env, char *pwd_now);
char **my_cd(char **cmd, char **env);

void initialize_array(char **array, int size);
char **my_calloc_ar(int size);

void initialize_string(char *str, int size);
char *my_calloc_str(int size);

void set_saved_cmd(char *str);
char *get_saved_cmd(void);
char *my_cmd(char *str);

void print_env(char **env);
char **my_env(char **cmd, char **env);

char *build_error_msg(char *cmd, char *str, int err);
void my_error(char *cmd, int err);

void free_resources(char **cmd, char **env);
char **my_exit(char **cmd, char **env);

void free_string_array(char **str);
void my_free(char **str);

char *find_variable_in_env(char **env, char *temp);
char *my_getenv(char **env, char *variable);

void process_commands(char **cmd, char **env);
void handle_prompt(void);
int read_input(char **temp, size_t *len);
void my_input(char **env);

void handle_pipes(char **cmd, char **env, int *fd);
void handle_parent_process(int pid, int *status, char **cmd, char **env);
void my_pipe(char *str, char **env);

void write_string_to_fd(char *str, int fd);
void my_putstr(char *str, int fd);

char **my_setenv(char **cmd, char **env);

void concatenate_string(char *dest, char *src, int start);
int copy_string(char *dest, char *src);
char *my_strcat_path(char *str1, char *str2);

void concatenate_str(char *dest, char *src, int start);
int copy_str(char *dest, char *src);
char *my_strcat(char *str1, char *str2);

int compare_lengths(char *str1, char *str2);
int compare_chars(char *str1, char *str2);
int my_strcmp(char *str1, char *str2);

void cp_string(char *dest, char *src);
char *my_strdup(char *str);

int count_elements(char **str);
int my_strlen_array(char **str);

int count_characters(char *str);
int my_strlen(char *str);

int compare_strings(char *str1, char *str2, int n);
int validate_length(char *str1, char *str2, int n);
int my_strncmp(char *str1, char *str2, int n);

void skip_initial_separators(char *str, int *i, char sep);
void skip_separators(char *str, int *i, char sep);
void fill_result(char *str, char sep, char **result, int *i);
char **my_strtab(char *str, char sep);

void copy_tab_elements(char **dest, char **src);
char **my_tabdup(char **str);

void shift_env_lines(char **env, int start);
int is_matching_env(char *env_entry, char *value);
char **my_unsetenv(char **cmd, char **env);

void replace_chars_in_string(char *str, char find, char replace);
void replace_char(char *str, char find, char replace);

void set_env(char **env);
char **get_env(void);
char **single_env(char **env);

void set_old(char *str);
char *get_old(void);
char *single_old(char *str);

void write_error_prefix(void);
void write_error_message(char *str);
void write_error_suffix(void);
void write_my_error(char *str);

void truncate_at_first_space(char *str);
char *remove_leading_spaces(char *str);
char *clean_string(char *str);

void handle_signal(int sig);
void check_signal(int signal);
void exec_cmd(char *path, char **cmd, char **env);
void handle_non_builtin(char **cmd, char **env, int *status);
void my_treat_cmd(char **cmd, char **env);

int check_builtin(char **built_in, char *cmd);
int is_builtin(char *cmd);
#endif /*MY_LIB_INCL*/
