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
typedef struct param_s {
    int nb;
    int palindrome;
    int base;
    int min;
    int max;
    int iteration;
} param_t;


/*prototype*/
void display_help(void);

int option_handling(int argc, char **argv);
int error_handling(int argc, char **argv);

int handle_palindrome(int argc, char **argv);

int option_n(param_t *param, int nb);
int option_p(param_t *param);

void my_revstr(char *str);
int is_num(char *str);
int my_put_nbr(int nb);
int my_putstr_error(const char *str);

#endif /* !SYN_H_ */
