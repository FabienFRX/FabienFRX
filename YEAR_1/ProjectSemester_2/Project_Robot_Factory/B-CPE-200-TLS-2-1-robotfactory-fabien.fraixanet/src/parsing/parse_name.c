/*
** EPITECH PROJECT, 2024
** B-CPE-200-TLS-2-1-robotfactory-fabien.fraixanet [WSL: Debian]
** File description:
** parse_file
*/

#include "../../include/asm.h"

static ssize_t find_quote_end(const char *buffer, ssize_t start)
{
    ssize_t size = 0;

    while (buffer[start + size] != '\0' && buffer[start + size] != '"')
        size++;
    return start + size;
}

static char *extract_name(const char *buffer, ssize_t start, ssize_t end)
{
    ssize_t size = end - start;
    char *name = malloc(sizeof(char) * (size + 1));

    if (name == NULL)
        return NULL;
    for (ssize_t i = 0; i < size; i++)
        name[i] = buffer[start + i];
    name[size] = '\0';
    return name;
}

static void update_head(testa_t *head, ssize_t name_end, ssize_t j)
{
    head->name[0] = extract_name(head->data, j, name_end);
    if (head->name[0] == NULL) {
        head->status = 84;
        return;
    }
    head->data_start = head->data + name_end + 1;
    head->data_len -= (name_end + 1 - j);
}

void parse_name(testa_t *head)
{
    char *buffer = head->data;
    ssize_t j = 0;
    FILE *file = head->file;
    ssize_t name_end = 0;

    while (buffer[j] != '\0' && buffer[j] != '"')
        j++;
    if (buffer[j] == '\0') {
        head->status = 84;
        return;
    }
    j++;
    name_end = find_quote_end(buffer, j);
    if (name_end == j) {
        head->status = 84;
        return;
    }
    update_head(head, name_end, j);
    fseek(file, name_end + 1, SEEK_SET);
}
