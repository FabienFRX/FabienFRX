/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-sbmlparser-fabien.fraixanet
** File description:
** file_func
*/

#include "../include/syn.h"
char *get_value(node_t *info, char *str, int first)
{
    info->name = get_next_word_size(str, first);
}

char *file_reader(char const *filepath)
{
    FILE *file = fopen(filepath, "r");
    char *line = NULL;
    size_t len = 0;

    if (file == NULL)
        return (NULL);
    while ((getline(&line, &len, file)) != -1) {
        printf("%s", line);
    }
    fclose(file);
    return (line);
}
