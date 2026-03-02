/*
** EPITECH PROJECT, 2024
** B-CPE-200-TLS-2-1-robotfactory-fabien.fraixanet [WSL: Debian]
** File description:
** parse_comment
*/

#include "../../include/asm.h"

static char *remove_line(char *buffer, ssize_t j)
{
    while (buffer[j] != '\0' && buffer[j] != '\n')
        j++;
    if (buffer[j] == '\n')
        j++;
    return buffer + j;
}

static ssize_t find_quote_end(const char *buffer, ssize_t start)
{
    ssize_t size = 0;

    while (buffer[start + size] != '\0' && buffer[start + size] != '"')
        size++;
    return start + size;
}

static void extract_comment(const char *buffer, char **comment, ssize_t start)
{
    ssize_t size = find_quote_end(buffer, start) - start;

    *comment = malloc(sizeof(char) * (size + 1));
    if (*comment == NULL) {
        return;
    }
    for (size_t i = 0; i < size; i++)
        (*comment)[i] = buffer[start + i];
    (*comment)[size] = '\0';
}

void parse_comment(testa_t *head)
{
    char *buffer = head->data;
    ssize_t j = 0;

    buffer = remove_line(buffer, j);
    while (buffer[j] != '\0' && buffer[j] != '"')
        j++;
    if (buffer[j] == '\0') {
        head->status = 84;
        return;
    }
    j++;
    extract_comment(buffer, &(head->comment[0]), j);
    head->data_start = buffer + j + my_strlen(head->comment[0]) + 1;
    head->data_len -= (my_strlen(head->comment[0]) + 1 + j);
}
