/*
** EPITECH PROJECT, 2024
** robot_factory
** File description:
** qwerty
*/

#ifndef ASM_H_
    #define ASM_H_
    #include "my_printf.h"
    #include "my.h"
    #include "op.h"
    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <stdbool.h>

typedef struct label_dict_s label_dict_t;
typedef struct testa_s {
    char *data;
    char *data_start;
    char *name[PROG_NAME_LENGTH + 1];
    char *comment[COMMENT_LENGTH + 1];
    FILE *file;
    int data_len;
    int command_count;
    int status;
    label_dict_t *label_list;
} testa_t;

typedef struct label_dict_s {
    char *label_name;
    int *occurrence;
    int *cmd_occurrence;
    int *occurrence_len;
    int occurrence_count;
    int location;
    struct label_dict_s *next_label;
    struct label_dict_s *prev_label;
} label_dict_t;

static const char MAGIC_HEADER[4] = {0, 234, 131, 243};

//src/main.c
int main(int argc, char *argv[]);
int my_asm(testa_t *head);

//src/initialize_structs.c
void initialize_header(testa_t *head, char *argv[]);
void create_label(label_dict_t **lista, char *label_name);
void push_label(label_dict_t **lista, label_dict_t *block);

//src/robustness.c
int robustness(int argc, char *argv[]);
int display_help(void);

//src/error_management.c
void my_putchar_error(char c);
int my_putstr_error(const char *str);
int my_put_nbr_error(int nb);

//src/file_reader.c
char *file_reader(char const *filepath);

//src/parsing/parse_name.c
void parse_name(testa_t *head);

//src/parsing/parse_comment.c
void parse_comment(testa_t *head);

//src/parsing/clear_comments.c
char *get_filtered_text(testa_t *head);

//src/file_converter.c
char *convert_in_cor(testa_t *head);

#endif /* !ASM_H_ */
