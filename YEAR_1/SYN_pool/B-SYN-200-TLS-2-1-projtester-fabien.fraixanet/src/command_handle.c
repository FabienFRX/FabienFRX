/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-projtester-fabien.fraixanet
** File description:
** command_handle
*/

#include "../include/syn.h"

/*function to detect file .tdf*/
static int is_test_file(char *filename)
{
    int len = strlen(filename);
    char *last_four = &filename[len - 4];

    if (strcmp(last_four, ".tdf") == 0)
        return 0;
    return 1;
}

/*display directories with '[]' when detect '/'*/
static void display_directories(char *directory, char *path)
{
    int len_dir = strlen(directory);
    int last = 0;
    int i = 0;
    int j = 0;

    for (int i = strlen(directory); path[i] != '\0'; i++)
        if (path[i] == '/')
            last = i;
    for (i = len_dir + 1; path[i] != '\0' && i < last; i++) {
        printf("[");
        for (j = i; path[j] != '\0' && path[j] != '/'; j++)
            printf(&path[j]);
        i = j;
        printf("] ");
    }
    for (; path[i] != '\0' && i < strlen(path) - 4; i++)
        printf(&path[i]);
    printf(": ");
}

static char *concat_path(char *path, char *name)
{
    char *ret = NULL;
    int length = strlen(path) + strlen(name) + 2;

    ret = malloc(sizeof(char) * length);
    if (ret == NULL)
        return (NULL);
    strcpy(ret, path);
    ret[strlen(path)] = '/';
    ret[strlen(path) + 1] = '\0';
    strcat(ret, name);
    ret[length - 1] = '\0';
    return ret;
}

/*manage command in fork*/
static void exec(char *command, char **args)
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
        return;
    if (pid == 0) {
        execv(command, args);
        exit(0);
    } else
        wait(NULL);
}

static int array_len(char **array)
{
    int len = 0;

    if (array == NULL)
        return 0;
    for (len = 0; array[len] != NULL; len++);
    return len;
}

/*exec command by his path*/
void exec_command_path(char *command, char **args)
{
    char *path[3] = {"/bin", "/usr/bin", NULL};
    char *tmp = NULL;

    for (int i = 0; path[i] != NULL; i++) {
        tmp = concat_path(path[i], command);
        if (access(tmp, F_OK) == 0) {
            exec(tmp, args);
            free(tmp);
            break;
        }
        free(tmp);
    }
}

/*display command func*/
void exec_command(char *command, char *content, char *input)
{
    char **params = my_str_to_word_array(content, " ");
    char **args = malloc(sizeof(char *) * array_len(params) + 3);

    args[0] = strdup(command);
    for (int i = 0; params != NULL && params[i] != NULL; i++) {
        args[i + 1] = strdup(params[i]);
    }
    if (input != NULL)
        args[array_len(params) + 1] = strdup(input);
    else
        args[array_len(params) + 1] = NULL;
    args[array_len(params) + 2] = NULL;
    if (strncmp(command, "/", 1) == 0 || strncmp(command, ".", 1) == 0)
        exec(command, args);
    else
        exec_command_path(command, args);
}

void command_handle(tree_t *tree, char *directory, char *command)
{
    for (; tree != NULL; tree = tree->next) {
        if (tree->path == NULL)
            continue;
        if (is_test_file(tree->path) == 0) {
            display_directories(directory, tree->path);
            exec_command(command, tree->content[0], tree->content[1]);
        }
    }
}
