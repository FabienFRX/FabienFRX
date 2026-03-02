/*
** EPITECH PROJECT, 2024
** Connection
** File description:
** azerty
*/

#include "../include/my_navy.h"

player_t*p;

void signal_handler(int sig, siginfo_t *dope, void *context)
{
    (void)context;
    p->pid_2 = dope->si_pid;
    if (sig == SIGUSR1 && p->check == 0)
        p->check = 1;
    if (sig == SIGUSR2 && p->check)
        p->check = 1;
}

void connection(void)
{
    struct sigaction sa;
    sigset_t mask;

    sigemptyset(&mask);
    sa.sa_mask = mask;
    sa.sa_sigaction = signal_handler;
    sa.sa_flags = SA_SIGINFO;
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        my_printf("Error setting up signal handler for SIGUSR1\n");
        exit(84);
    }
    if (sigaction(SIGUSR2, &sa, NULL) == -1) {
        my_printf("Error setting up signal handler for SIGUSR2\n");
        exit(84);
    }
    while (p->check == 0)
        usleep(100);
}
