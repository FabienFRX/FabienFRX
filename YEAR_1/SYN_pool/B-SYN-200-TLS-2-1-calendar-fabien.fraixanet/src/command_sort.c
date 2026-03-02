/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-calendar-fabien.fraixanet
** File description:
** command_sort
*/

#include "syn.h"

// MEETINGS
void info_meeting(meeting_t *meets, employee_t *employees, char *line)
{
    char **line_split = my_str_to_word_array(line, " ");
    meeting_t *tmp = meets;

    if (line_split == NULL)
        return;
    if (array_len(line_split) == 2)
        for (; tmp != NULL; tmp = tmp->next)
            display_meeting(meets, employees);
    else {
        for (int i = 2; line_split[i] != NULL; i++) {
            tmp = find_meeting(meets, atoi(line_split[i]));
            display_meeting(tmp, employees);
        }
    }
}

// EMPLOYEES
static void display_all_e(employee_t *employees, meeting_t *meets)
{
    employee_t *tmp = employees;

    for (; tmp != NULL; tmp = tmp->next) {
        display_employee(employees, tmp->id, meets);
    }
}

void info_employee(employee_t *employees, meeting_t *meets, char *line)
{
    char **line_split = my_str_to_word_array(line, " ");

    if (line_split == NULL)
        return;
    if (array_len(line_split) == 2)
        display_all_e(employees, meets);
    else {
        for (int i = 2; line_split[i] != NULL; i++) {
            display_employee(employees, atoi(line_split[i]), meets);
        }
    }
}
