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
typedef struct sequence_s {
    char *key;
    char *value;
    struct sequence_s *next;
} sequence_t;

/*prototype*/
void display_help(void);

int check_flag(char *opt, sequence_t *seq);
int error_handling(int argc, char **argv);

sequence_t *get_file(void);

int option_1(sequence_t *seq);
int option_2(sequence_t *seq);
int option_3(sequence_t *seq);
int option_5(sequence_t *seq);

char *substr(char *str, int begin, int length);

int is_num(char *str);
void my_putchar(char c);
int my_putstr(const char *str);
int my_put_nbr(int nb);
int my_putstr_error(const char *str);

#endif /* !SYN_H_ */
