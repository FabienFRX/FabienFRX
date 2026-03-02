/*
** EPITECH PROJECT, 2024
** B-CPE-200-TLS-2-1-amazed-fabien.fraixanet [WSL: Debian]
** File description:
** parsing4
*/

#include "../../include/amazed.h"

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

static void process_line(const char *line, ssize_t read_bytes)
{
    for (ssize_t i = 0; i < read_bytes; i++) {
        if (i > 0 && i < read_bytes - 1
            && is_start_comment(line[i - 1], line[i], line[i + 1])) {
            my_putchar('\n');
            break;
        } else {
            my_putchar(line[i]);
        }
    }
}

void print_filtered_text(void)
{
    char *line = NULL;
    size_t size = 0;
    ssize_t read_bytes;

    while (1) {
        read_bytes = getline(&line, &size, stdin);
        if (read_bytes == -1) {
            break;
        }
        if (!should_skip_line(line, read_bytes)) {
            process_line(line, read_bytes);
        }
    }
    free(line);
}
