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

typedef struct employee_s {
    int id;
    char *last_name;
    char *name;
    char *job;
    char *zipcode;
    struct employee_s *next;
} employee_t;

typedef struct meeting_s {
    int id;
    char *day;
    char *zipcode;
    int *employees_id;
    int size;
    struct meeting_s *next;
} meeting_t;

/*prototype*/
void display_help(void);

void loop_till_end(void);

employee_t *init_employee(char *line);
employee_t *add_employee(employee_t *employees, char *line);
employee_t *delete_employee(employee_t *employees, int id);
employee_t *find_employee(employee_t *employees, int id);

meeting_t *init_meeting(char *line);
meeting_t *add_meeting(meeting_t *meetings, char *line);
meeting_t *delete_meeting(meeting_t *meetings, int id);
meeting_t *find_meeting(meeting_t *meetings, int id);

void new_employee(employee_t **employees, char *line);
void new_meeting(meeting_t **meetings, char *line);
void fire(employee_t **employees, char *line);

void display_meeting(meeting_t *meeting, employee_t *employees);
void display_employee(employee_t *employees, int id, meeting_t *meets);

int check_invited(meeting_t *meet, int id);
void info_employee(employee_t *employees, meeting_t *meets, char *line);
void info_meeting(meeting_t *meets, employee_t *employees, char *line);

int array_len(char **arr);
int is_num(char *str);
void my_putchar(char c);
int my_putstr(const char *str);
int my_put_nbr(int nb);
int my_putstr_error(const char *str);
int my_put_nbr_error(int nb);
char **my_str_to_word_array(char *str, char *stop);

#endif /* !SYN_H_ */
