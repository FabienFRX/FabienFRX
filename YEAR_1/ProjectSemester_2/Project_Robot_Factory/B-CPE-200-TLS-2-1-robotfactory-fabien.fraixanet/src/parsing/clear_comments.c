/*
** EPITECH PROJECT, 2024
** B-CPE-200-TLS-2-1-robotfactory-fabien.fraixanet [WSL: Debian]
** File description:
** clear_comments
*/

#include "../../include/asm.h"

static bool is_start_comment(char ch1, char ch2, char ch3)
{
    if (ch2 != '#')
        return false;
    if (ch1 != '#' && ch2 == '#' && ch3 != '#')
        return true;
    return false;
}

static bool is_line_comment(char ch1, char ch2)
{
    if (ch1 == '#' && ch2 != '#')
        return true;
    return false;
}

static bool should_skip_line(const char *line, ssize_t read_bytes)
{
    if (read_bytes > 1 && is_line_comment(line[0], line[1])) {
        return true;
    }
    return false;
}

static char *process_line(const char *line, ssize_t read_bytes)
{
    char *cleaned_line = malloc(read_bytes + 1);
    size_t cleaned_index = 0;

    if (!cleaned_line) {
        return NULL;
    }
    for (ssize_t i = 0; i < read_bytes; i++) {
        if (i > 0 && i < read_bytes - 1
            && is_start_comment(line[i - 1], line[i], line[i + 1])) {
            cleaned_line[cleaned_index++] = '\n';
            break;
        } else {
            cleaned_line[cleaned_index++] = line[i];
        }
    }
    cleaned_line[cleaned_index] = '\0';
    return cleaned_line;
}

static char *process_line_and_concatenate(char *line, ssize_t length,
    char *filtered_text, size_t *filtered_size)
{
    char *cleaned_line = process_line(line, length);
    char *new_filtered_text = NULL;
    size_t cleaned_length = 0;

    if (cleaned_line) {
        cleaned_length = my_strlen(cleaned_line);
        new_filtered_text = realloc(filtered_text,
            *filtered_size + cleaned_length + 1);
        if (!new_filtered_text) {
            free(cleaned_line);
            free(line);
            return NULL;
        }
        filtered_text = new_filtered_text;
        my_strcat(filtered_text, cleaned_line);
        *filtered_size += cleaned_length;
        free(cleaned_line);
    }
    return filtered_text;
}

static char *process_file(FILE *stream)
{
    char *filtered_text = NULL;
    size_t filtered_size = 0;
    ssize_t read_bytes;
    char *line = NULL;
    size_t line_size = 0;

    while (1) {
        line = NULL;
        line_size = 0;
        read_bytes = getline(&line, &line_size, stream);
        if (read_bytes == -1) {
            free(line);
            break;
        }
        if (!should_skip_line(line, read_bytes))
            filtered_text = process_line_and_concatenate(line, read_bytes,
                filtered_text, &filtered_size);
        free(line);
    }
    return filtered_text;
}

char *get_filtered_text(testa_t *head)
{
    FILE *stream = head->file;
    char *filtered_text = NULL;

    if (stream == NULL)
        return NULL;
    filtered_text = process_file(stream);
    return filtered_text;
}
