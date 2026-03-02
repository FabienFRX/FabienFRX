/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-projtester-fabien.fraixanet
** File description:
** tree_handle
*/

#include "../include/syn.h"

static int is_dir(char *path)
{
    struct stat statbuf;

    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

static tree_t *init_node(void)
{
    tree_t *node = malloc(sizeof(tree_t));

    if (node == NULL)
        return (NULL);
    node->next = NULL;
    node->content = NULL;
    node->path = NULL;
    return (node);
}

/*adding new node at end of list*/
static tree_t *add_node(tree_t *tree, char *path)
{
    tree_t *node = NULL;

    node = init_node();
    node->path = strdup(path);
    if (tree == NULL) {
        *tree = *node;
        return tree;
    } else {
        for (; tree->next != NULL; tree = tree->next);
        tree->next = node;
        return (tree->next);
    }
}

static void get_file_data(char *path, tree_t *tree)
{
    FILE *fd = fopen(path, "r");
    char *line = NULL;
    char **tmp = NULL;
    size_t s = 0;

    if (fd == NULL)
        return;
    tree->content = calloc(3, sizeof(char *));
    for (int i = getline(&line, &s, fd); i != -1; i = getline(&line, &s, fd)) {
        tmp = my_str_to_word_array(line, ":\n");
        if (strncmp(line, "ARGS:", 5) == 0)
            tree->content[0] = strdup(tmp[1]);
        if (strncmp(line, "INPUT:", 6) == 0)
            tree->content[1] = strdup(tmp[1]);
        free_array(tmp);
    }
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

tree_t *tree_handle(char *directory, tree_t *tree)
{
    DIR *fd = opendir(directory);
    struct dirent *lin = NULL;
    char *tmp = NULL;

    if (fd == NULL)
        return NULL;
    for (lin = readdir(fd); lin != NULL; lin = readdir(fd)) {
        if (strcmp(".", lin->d_name) == 0 || strcmp("..", lin->d_name) == 0)
            continue;
        tmp = concat_path(directory, lin->d_name);
        if (is_dir(tmp)) {
            tree = add_node(tree, tmp);
            tree_handle(tmp, tree);
        } else {
            tree = add_node(tree, tmp);
            get_file_data(tmp, tree);
        }
        free(tmp);
    }
    closedir(fd);
}
