/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-palindrome-fabien.fraixanet
** File description:
** option_p
*/

#include "../include/syn.h"

/* make_iteration function: - check iteration for iteration display */
static int make_iteration(param_t *param, int nb)
{
    int rev_num = 0;
    int tmp = nb;

    while (nb > 0) {
        rev_num = rev_num * param->base + nb % param->base;
        nb = nb / param->base;
    }
    return (tmp + rev_num);
}

/* display_option_p function: - display result the subject asked */
static void display_option_p(param_t *param, int nb)
{
    printf("%d leads to %d in %d iteration(s) in base %d\n", nb,
        param->palindrome, param->iteration, param->base);
}

/* loop_option_p function: - display result of option p
** by calling display_option_p function
** - check iteration of option_p by calling make_iteration
*/
static int loop_option_p(param_t *param, int nb)
{
    int res = 0;

    for (int i = nb; param->iteration <= param->max; param->iteration++) {
        if (param->iteration >= param->min && param->palindrome == i) {
            display_option_p(param, nb);
            res++;
        }
        i = make_iteration(param, i);
    }
    return (res);
}

/* check_pal function: - check if it's a palindrome */
static int check_pal(param_t *param, int nb)
{
    int rev_num = 0;
    int tmp = nb;

    while (nb > 0) {
        rev_num = rev_num * param->base + nb % param->base;
        nb = nb / param->base;
    }
    if (tmp == rev_num)
        return (0);
    return (1);
}

/* option_p function: - check if it's not a palindrome
** by calling check_pal function
** - check iteration of option_p by calling loop_option_p function
** - check if not possible display no solution
*/
int option_p(param_t *param)
{
    int res = 0;

    if (check_pal(param, param->palindrome) == 1)
        return (84);
    for (int i = 0; i <= param->palindrome; i++) {
        res += loop_option_p(param, i);
        param->iteration = 0;
    }
    if (res == 0) {
        printf("no solution\n");
    }
    return (0);
}
