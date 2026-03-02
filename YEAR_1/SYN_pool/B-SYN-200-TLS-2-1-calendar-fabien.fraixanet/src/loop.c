/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-calendar-fabien.fraixanet
** File description:
** loop
*/

#include "syn.h"

// pour afficher avec un tri
// faire comme check_command mais regarder str_to_word_array(line)[1]
// et du coup 1 fonction par tri + affichage apres le tri

static void display_str_meeting(employee_t *employee, meeting_t *meeting)
{
    my_putstr("******************************\n");
    my_putstr("MEETING\n");
    my_putstr("date: ");
    my_putstr(meeting->day);
    my_putstr("\n");
    my_putstr("place: ");
    my_putstr(meeting->zipcode);
    my_putstr("\n");
}

void display_meeting(meeting_t *meeting, employee_t *employees)
{
    employee_t *employee = NULL;

    display_str_meeting(employee, meeting);
    if (meeting->size < 1)
        return;
    my_putstr("with ");
    for (int i = 0; i < meeting->size; i++) {
        employee = find_employee(employees, meeting->employees_id[i]);
        if (employee != NULL) {
            my_putstr(employee->name);
            my_putstr(" ");
            my_putstr(employee->last_name);
            my_putstr("\n");
        }
        if (i != meeting->size - 1)
            my_putstr(",");
    }
    my_putstr("\n");
}

int check_invited(meeting_t *meet, int id)
{
    for (int i = 0; i < meet->size; i++) {
        if (meet->employees_id[i] == id)
            return 0;
    }
    return 1;
}

static void display_str_employee(employee_t *employees, int id,
    meeting_t *meets)
{
    my_putstr("******************************\n");
    my_putstr(employees->name);
    my_putstr(" ");
    my_putstr(employees->last_name);
    my_putstr("\n");
    my_putstr("position: ");
    my_putstr(employees->job);
    my_putstr("\n");
    my_putstr("city: ");
    my_putstr(employees->zipcode);
    my_putstr("\n");
}

static void display_loop_employee(employee_t *employees, int id,
    meeting_t *meets)
{
    employee_t *tmp = NULL;

    my_putstr("meeting on ");
    my_putstr(meets->day);
    my_putstr(", ");
    my_putstr(meets->zipcode);
    my_putstr(" with ");
    for (int i = 0; i < meets->size; i++) {
        if (meets->employees_id[i] == id)
            continue;
        tmp = find_employee(employees, meets->employees_id[i]);
        if (tmp != NULL) {
            my_putstr(tmp->name);
            my_putstr(" ");
            my_putstr(tmp->last_name);
            if (i != meets->size - 1)
                my_putstr(", ");
        }
    }
    my_putstr("\n");
}

void display_employee(employee_t *employees, int id, meeting_t *meets)
{
    employee_t *employee = NULL;
    int is_employee = 1;

    for (employee = employees; employee != NULL; employee = employee->next)
        if (employee->id == id)
            break;
    if (employee == NULL)
        return;
    display_str_employee(employee, id, meets);
    for (; meets != NULL; meets = meets->next) {
        is_employee = check_invited(meets, id);
        if (is_employee == 1)
            continue;
        display_loop_employee(employees, id, meets);
    }
        my_putstr("\n");
        is_employee = 1;
}

//    if (strncmp(line, "invite", 6) == 0)
//        invite(e);
//    if (strncmp(line, "exclude", 7) == 0)
//        exclude(e, meet);
static void check_command(char *line, employee_t **e, meeting_t **meet)
{
    if (strncmp(line, "new_employee", 12) == 0)
        new_employee(e, line);
    if (strncmp(line, "new_meeting", 11) == 0)
        new_meeting(meet, line);
    if (strncmp(line, "fire", 4) == 0)
        fire(e, line);
    if (strncmp(line, "info_meetings", 13) == 0)
        info_meeting(*meet, *e, line);
    if (strncmp(line, "info_employees", 14) == 0)
        info_employee(*e, *meet, line);
}

void loop_till_end(void)
{
    char *buff = NULL;
    size_t size;
    employee_t *employees = NULL;
    meeting_t *meets = NULL;

    for (int i = getline(&buff, &size, stdin); i != -1;
        i = getline(&buff, &size, stdin)) {
        if (buff[strlen(buff) - 1] == '\n')
            buff[strlen(buff) - 1] = '\0';
        if (0 == strcmp(buff, "END"))
            break;
        check_command(buff, &employees, &meets);
    }
}
