/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-sbmlparser-fabien.fraixanet
** File description:
** syn
*/

#ifndef SYN_H_
    #define SYN_H_

/*include*/
    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <sys/wait.h>
    #include <stdbool.h>
    #include <dirent.h>

/*structure*/
/*
    root
        root/file
        root/subDir
            root/subDir/file
*/
typedef struct tree_s {
    char *path;
    char **content;
    struct tree_s *next;
} tree_t;



/*prototype*/
void display_help(void);
void display_tree(char *directory, tree_t *tree);

tree_t *tree_handle(char *directory, tree_t *tree);

int swap_ptr(tree_t *current, tree_t *next);
void sort(tree_t *tree);

char **my_str_to_word_array(char *str, char *stop);

int is_num(char *str);
int my_put_nbr(int nb);
int my_putstr_error(const char *str);

void free_array(char **arr);
void free_tree(tree_t *tree);

void command_handle(tree_t *tree, char *directory, char *command);

#endif /* !SYN_H_ */
