/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-palindrome-fabien.fraixanet
** File description:
** option_n
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

/* display_option_n function: - display result the subject asked */
static void display_option_n(param_t *param, int nb)
{
    printf("%d leads to %d in %d iteration(s) in base %d\n", param->nb, nb,
        param->iteration, param->base);
}

/* option_n function: - check if is a palindrome by calling check_pal function
** - display result of option n by calling display_option_n function
** - check iteration of option_n by calling make_iteration
** - check if not possible display no solution
*/
int option_n(param_t *param, int nb)
{
    for (; param->iteration <= param->max; param->iteration++) {
        if (param->iteration >= param->min && check_pal(param, nb) == 0) {
            display_option_n(param, nb);
            return (0);
        }
        nb = make_iteration(param, nb);
    }
    printf("no solution\n");
    return (0);
}
