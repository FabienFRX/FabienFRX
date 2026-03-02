/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-palindrome-fabien.fraixanet
** File description:
** main
*/

#include "../include/syn.h"

/*
** check_more_option function: - check if for each flag by two is existent
** - check if for each after_flag by two is a num
** - check if nothing after flag
*/
int check_more_options(char **argv)
{
    for (int i = 3; argv[i] != NULL; i += 2) {
        if (strcmp(argv[i], "-b") != 0 && strcmp(argv[i], "-imin") != 0 &&
            strcmp(argv[i], "-imax") != 0 && (strcmp(argv[i], "-n") == 0 ||
            strcmp(argv[i], "-p") == 0))
            return (84);
    }
    for (int i = 2; argv[i] != NULL; i += 2) {
        if (is_num(argv[i]) != 0)
            return (84);
        if (argv[i + 1] == NULL)
            break;
    }
    return (0);
}

/*
** option_handling function: - check if -n and -p not put together
** - check if each 2nd arg is a num
** - call check_more_option function
*/
int option_handling(int argc, char **argv)
{
    if (strcmp(argv[1], "-n") != 0 && strcmp(argv[1], "-p") != 0)
        return (84);
    if (is_num(argv[2]) != 0)
        return (84);
    if (argc > 3)
        return check_more_options(argv);
    return (0);
}

/*
** error_handling function: - check if -h
** - check if arg is coming by two
** - call option_handling function
*/
int error_handling(int argc, char **argv)
{
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        display_help();
        exit(0);
    }
    if ((argc % 2) != 1 || option_handling(argc, argv) == 84) {
        return (84);
    }
    return (0);
}

/*
** main function: - check if no arg
** - call function for error handling
** - call function for palindrome
*/
int main(int argc, char **argv)
{
    if (argc <= 1)
        return (84);
    if (error_handling(argc, argv) == 84) {
        my_putstr_error("invalid argument\n");
        return (84);
    }
    if (handle_palindrome(argc, argv) == 84)
        return (84);
    return (0);
}
