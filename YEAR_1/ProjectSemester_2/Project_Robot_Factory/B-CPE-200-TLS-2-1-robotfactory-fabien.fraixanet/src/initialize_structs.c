/*
** EPITECH PROJECT, 2024
** Robot Factory
** File description:
** asm project
*/

#include "../include/asm.h"

void initialize_header(testa_t *head, char *argv[])
{
    head->data = file_reader(argv[1]);
    head->data_start = head->data;
    head->data_len = my_strlen(head->data);
    head->status = 0;
    head->command_count = 0;
    head->label_list = NULL;
    head->file = fopen(argv[1], "r");
    head->name[0] = NULL;
    head->comment[0] = NULL;
}
