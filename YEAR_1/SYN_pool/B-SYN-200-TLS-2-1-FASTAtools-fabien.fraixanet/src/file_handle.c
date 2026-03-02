/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-FASTAtools-fabien.fraixanet
** File description:
** file_handle
*/

#include "../include/syn.h"

int get_line_size(char *line)
{
    int i = 0;

    for (; line[i] != '\0'; i++) {
        if (line[i] != '\n')
            continue;
        else
            break;
    }
    return (i);
}

int get_sequence_size(char *line)
{
    int count = 0;

    for (int i = 0; line[i] != '\0' && line[i] != '\n'; i++) {
        if ((line[i] >= 'a' && line[i] <= 'z') || (line[i] >= 'A' &&
            line[i] <= 'Z') || line[i] == ' ')
            count++;
    }
    return (count);
}

static void add_char_to_sequence(char *line, sequence_t *seq, int beg, int fir)
{
    if (fir == 1) {
        seq->value[beg] = ' ';
        beg++;
    }
    for (int i = 0; line[i] != '\0' && line[i] != '\n'; i++) {
        if (line[i] >= 'a' && line[i] <= 'z') {
            seq->value[beg] = line[i] - 32;
            beg++;
        }
        if ((line[i] >= 'A' && line[i] <= 'Z') || line[i] == ' ') {
            seq->value[beg] = line[i];
            beg++;
        }
    }
}

static sequence_t *init_new_sequence(char *line, int line_size)
{
    sequence_t *new = malloc(sizeof(sequence_t));

    if (new == NULL)
        return (NULL);
    new->next = NULL;
    new->value = NULL;
    new->key = malloc(sizeof(char) * (line_size + 1));
    if (new->key == NULL)
        return (NULL);
    for (int i = 0; line[i] != '\0' && line[i] != '\n'; i++) {
        new->key[i] = line[i];
    }
    new->key[line_size] = '\0';
    return (new);
}

/* we use a ptr on seq and head to give them the adress of the list*/
static void insert_node(char *line, sequence_t **seq, sequence_t **head)
{
    if (*seq == NULL) {
        *seq = init_new_sequence(line, get_line_size(line));
        *head = *seq;
    } else {
        (*seq)->next = init_new_sequence(line, get_line_size(line));
        *seq = (*seq)->next;
    }
}

static void fill_node(char *line, sequence_t *seq)
{
    int sequence_size = 0;
    int current_size = 0;

    if (seq == NULL)
        return;
    sequence_size = get_sequence_size(line);
    if (seq->value == NULL) {
        seq->value = malloc(sizeof(char) * (sequence_size + 1));
        add_char_to_sequence(line, seq, 0, 0);
        seq->value[sequence_size] = '\0';
    } else {
        current_size = strlen(seq->value);
        seq->value = realloc(seq->value, (sequence_size + current_size + 2));
        add_char_to_sequence(line, seq, current_size, 1);
        seq->value[sequence_size + current_size + 1] = '\0';
    }
}

sequence_t *get_file(void)
{
    char line[BUFSIZ];
    sequence_t *seq;
    sequence_t *head;

    if (isatty(STDIN_FILENO)) {
        return NULL;
    }
    while (fgets(line, BUFSIZ, stdin) != NULL){
        if (line[0] == '>') {
            insert_node(line, &seq, &head);
        } else {
            fill_node(line, seq);
        }
    }
    return head;
}
