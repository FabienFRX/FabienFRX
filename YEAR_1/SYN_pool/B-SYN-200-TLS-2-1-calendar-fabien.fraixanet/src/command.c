/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-calendar-fabien.fraixanet
** File description:
** command
*/

#include "syn.h"

void new_employee(employee_t **employees, char *line)
{
    if (*employees == NULL)
        *employees = init_employee(line);
    else
        add_employee(*employees, line);
}

void new_meeting(meeting_t **meetings, char *line)
{
    if (*meetings == NULL)
        *meetings = init_meeting(line);
    else
        add_meeting(*meetings, line);
}

/*split ta ligne
chercher le meeting mentionne en line_split[1]
    (invite idMeeting idEmployee1 [idEmployee2 . . . .])
check que meetings n est pas NULL
pour tout les elements a partir du 3eme index:
atoi
chercher si un employé a cet id
realloc meetings->employees_id
ajouter l id
*/
employee_t *invite(employee_t *employees, meeting_t *meetings, char *line)
{
        return NULL;
}

/*count = taille de meet->employees_id
parcourir meet->employees_id
si l id est dans line_split alors count--
return count
*/
static int get_new_meeting_employees_len(meeting_t *meet, char **line_split)
{
    return 0;
}

/*split ta ligne
chercher le meeting mentionne en line_split[1]
allouer un nouveau tableau de taille get_new_meeting_employees_len(meetings,
    line_split)
copier dedans tous les id qui ne sont pas dans line_split
free ancien tableau
assigner le nouveau a la place
*/
employee_t *exclude(employee_t *employees, meeting_t *meetings, char *line)
{
    return NULL;
}

void fire(employee_t **employees, char *line)
{
    char **line_split = my_str_to_word_array(line, " ");

    if (line_split == NULL || array_len(line_split) != 2)
        return;
    *employees = delete_employee(*employees, atoi(line_split[1]));
}
