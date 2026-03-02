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
    #include <stdbool.h>

/*structure*/
typedef struct pair_s {
    char *key;
    char *value;
} pair_t;

typedef struct node_s {
    char *name; // nom du noeud
    pair_t *attributs; // to get key="value"
    struct node *childs; // all elements below
} node_t;


/*prototype*/
int options(int argc, char **argv);

char *file_reader(char const *filepath);

void display_help(void);

int option_i(void);
int option_e(void);
int option_json(void);

int is_alph_num(char c);
int get_next_word_size(char const *str, int begin);
int my_putstr(char const *str);
int my_strcmp(char const *s1, char const *s2);
int my_strlen(char const *str);
int my_strncmp(char const *s1, char const *s2, int n);
void my_putchar_error(char c);
int my_putstr_error(const char *str);
int my_put_nbr_error(int nb);
char **my_str_to_word_array(char const *str);

#endif /* !SYN_H_ */
