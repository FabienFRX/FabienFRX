/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-FASTAtools-fabien.fraixanet
** File description:
** main
*/

#include "../include/syn.h"

int check_flag(char *opt, sequence_t *seq)
{
    if (strcmp(opt, "1") == 0)
        option_1(seq);
    if (strcmp(opt, "2") == 0)
        option_2(seq);
    if (strcmp(opt, "3") == 0)
        option_3(seq);
    if (strcmp(opt, "5") == 0)
        option_5(seq);
    return (0);
}

int error_handling(int argc, char **argv)
{
    int test_opt = 0;

    if (argc == 2 && strcmp(argv[1], "-h") == 0){
        display_help();
        exit(0);
    }
    if (argc != 2)
        return (84);
    test_opt = atoi(argv[1]);
    if (test_opt < 1 || test_opt > 7)
        return (84);
    return (0);
}

int main(int argc, char **argv)
{
    sequence_t *seq = NULL;

    if (error_handling(argc, argv) == 84)
        return (84);
    seq = get_file();
    if (seq == NULL) {
        return (84);
    }
    check_flag(argv[1], seq);
    return (0);
}
