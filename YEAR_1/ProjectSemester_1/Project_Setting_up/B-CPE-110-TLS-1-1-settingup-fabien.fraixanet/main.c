/*
** EPITECH PROJECT, 2023
** main
** File description:
** main test
*/

#include "include/my.h"

int size_of(char const *file)
{
    struct stat all_info;

    if (stat(file, &all_info) == -1){
        return (-84);
    }
    return all_info.st_size;
}

char *getmap(char *filename)
{
    int fd = open(filename, O_RDONLY);
    int lenght = size_of(filename);
    char *map;
    long int reading;

    if (fd == -1) {
        return (NULL);
    }
    map = malloc(sizeof(char) * (lenght + 1));
    if (map == NULL) {
        return (NULL);
    }
    reading = read(fd, map, lenght);
    close(fd);
    if (reading <= 0) {
        return (NULL);
    }
    map[lenght] = '\0';
    return (map);
}

int call_func(char *filename)
{
    char *file_content = NULL;
    char **map = NULL;

    file_content = getmap(filename);
    if (file_content == NULL) {
        return (84);
    }
    for (; *file_content != '\n'; file_content++) {
    }
    file_content++;
    map = my_str_to_word_array(file_content);
    replace_square(map);
    return (0);
}

int main(int argc, char **argv)
{
    if (argc != 2){
        return (84);
    }
    if (call_func(argv[1]) == 84) {
        return (84);
    }
    return (0);
}
