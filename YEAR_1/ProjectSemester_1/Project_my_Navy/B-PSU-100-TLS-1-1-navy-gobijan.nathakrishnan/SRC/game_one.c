/*
** EPITECH PROJECT, 2024
** Game_One
** File description:
** azerty
*/

#include "../include/my_navy.h"

player_t *p;

void multi_kill(int n1, int n2)
{
    kill(p->pid_2, SIGUSR2);
    for (int i = 0; i != n1; i++) {
        kill(p->pid_2, SIGUSR1);
        usleep(1000);
    }
    kill(p->pid_2, SIGUSR2);
    for (int i = 0; i != n2; i++) {
        kill(p->pid_2, SIGUSR1);
        usleep(1000);
    }
    kill(p->pid_2, SIGUSR2);
}

void handl_sig_answer(int sig, siginfo_t *inf, void *context)
{
    (void)context;
    (void)inf;
    if (sig == SIGUSR1)
        p->attack += 1;
    if (sig == SIGUSR2)
        p->attack += 2;
}

void receive_answer(void)
{
    struct sigaction sa;
    sigset_t mask;

    sigemptyset(&mask);
    sa.sa_mask = mask;
    sa.sa_sigaction = handl_sig_answer;
    sa.sa_flags = SA_SIGINFO;
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
        my_putstr("Error: cannot handle SIGUSR1\n");
    if (sigaction(SIGUSR2, &sa, NULL) == -1)
        my_putstr("Error: cannot handle SIGUSR2\n");
    while (p->attack == 0)
        usleep(100);
}

void handl_sig_mul_rec(int sig, siginfo_t *inf, void *context)
{
    (void)context;
    (void)inf;
    if (sig == SIGUSR1 && p->count == 0)
        p->check_eof += 1;
    if (sig == SIGUSR2)
        p->count += 1;
    if (sig == SIGUSR1) {
        if (p->count == 1)
            p->x += 1;
        if (p->count == 2)
            p->y += 1;
    }
}

int multi_receiver(void)
{
    struct sigaction sa;
    sigset_t mask;

    sigemptyset(&mask);
    sa.sa_mask = mask;
    sa.sa_sigaction = handl_sig_mul_rec;
    sa.sa_flags = SA_SIGINFO;
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
        my_printf("Error: cannot handle SIGUSR1\n");
    if (sigaction(SIGUSR2, &sa, NULL) == -1)
        my_printf("Error: cannot handle SIGUSR2\n");
    while (p->count != 3) {
        if (p->check_eof == 1)
            return (84);
        usleep(100);
    }
}

void map_update_e(map_t *m, int x, int y)
{
    if (p->attack == 1) {
        m->map_e[y][x] = 'x';
        my_printf("%c%c: hit\n\n", p->str[0], p->str[1]);
    }
    if (p->attack == 2) {
        m->map_e[y][x] = 'o';
        my_printf("%c%c: missed\n\n", p->str[0], p->str[1]);
    }
}

int game_one(map_t *m)
{
    p->attack = 0;
    p->x = 0;
    p->y = 0;
    if ((p->str = gnl()) == NULL)
        return quit_game();
    multi_kill(char_to_int2(p->str[0]), char_to_int1(p->str[1]) + 1);
    receive_answer();
    usleep(1000);
    empty_map_updated(m, char_to_int2(p->str[0]), char_to_int1(p->str[1]) + 1);
    my_printf("waiting for enemy's attack...\n");
    if (multi_receiver() == 84)
        return 0;
    p->count = 0;
    p->attack = check_map_coo(m->map);
    print_attack();
    usleep(1000);
    send_answer(p->attack);
    updated_map(m);
    usleep(1000);
    if (win_cond(m->map_e == 0))
        return 0;
    display_map(m->map, m->map_e);
    return 1;
}
