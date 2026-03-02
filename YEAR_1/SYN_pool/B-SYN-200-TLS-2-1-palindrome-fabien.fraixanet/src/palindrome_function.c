/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-palindrome-fabien.fraixanet
** File description:
** palindrome_function
*/

#include "../include/syn.h"

/* add_param function: - check for each param his limit ask by subject
** and set an error value */
void add_param(param_t *param, char **argv, int i)
{
    int value = atoi(argv[i + 1]);

    if (strcmp(argv[i], "-b") == 0) {
        if (value < 1 || value > 10)
            param->base = -2;
        else
            param->base = value;
    }
    if (strcmp(argv[i], "-imin") == 0) {
        if (value < 0)
            param->min = -2;
        else
            param->min = value;
    }
    if (strcmp(argv[i], "-imax") == 0) {
        if (value < 0)
            param->max = -2;
        else
            param->max = value;
    }
}

/* load_params function: -check -n param
** - check -p param
** - check for each param after -n or -p if limt is respected
** by calling add_param function
*/
void load_params(param_t *param, int argc, char **argv)
{
    if (strcmp(argv[1], "-n") == 0)
        param->nb = atoi(argv[2]);
    if (strcmp(argv[1], "-p") == 0)
        param->palindrome = atoi(argv[2]);
    if (argc < 4)
        return;
    for (int i = 3; argv[i] != NULL; i += 2)
        add_param(param, argv, i);
    return;
}

/* check_params function: - check if each param is error */
int check_params(param_t *param)
{
    if (param->base == -2 || param->min == -2 || param->max == -2 ||
        param->min > param->max)
        return (84);
    return (0);
}

/* handle_palindrome function: - check each flag and his default variable
** - call load_params function
** - check if is flag is non present by calling check_params function
** - call option_p function and option_n function
*/
int handle_palindrome(int argc, char **argv)
{
    param_t param = {.nb = 0, .palindrome = 0, .base = 10,
        .min = 0, .max = 100, .iteration = 0};

    load_params(&param, argc, argv);
    if (check_params(&param) == 84) {
        return (84);
    }
    if (param.palindrome)
        return option_p(&param);
    if (param.nb)
        return option_n(&param, param.nb);
    return (0);
}
