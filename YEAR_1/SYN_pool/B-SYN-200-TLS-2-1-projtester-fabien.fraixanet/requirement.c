/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-projtester-fabien.fraixanet
** File description:
** requirement
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdbool.h>

void my_ps_synthesis(void)
{
    char *argv[] = {"ps", 0};
    pid_t pid;

    pid = fork();
    if (pid == -1)
        return;
    if (pid == 0) {
        execv("/bin/ps", argv);
        exit(0);
    } else
        wait(NULL);
}
