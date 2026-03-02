/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-calendar-fabien.fraixanet
** File description:
** manage_employee
*/

#include "syn.h"

employee_t *init_employee(char *line)
{
    employee_t *employee = NULL;
    char **line_split = my_str_to_word_array(line, " ");

    if (array_len(line_split) != 6)
        return (NULL);
    employee = malloc(sizeof(employee_t));
    if (employee == NULL)
        return (NULL);
    employee->id = atoi(line_split[5]);
    employee->last_name = strdup(line_split[1]);
    employee->name = strdup(line_split[2]);
    employee->job = strdup(line_split[3]);
    employee->zipcode = strdup(line_split[4]);
    employee->next = NULL;
    return (employee);
}

employee_t *add_employee(employee_t *employees, char *line)
{
    employee_t *add_employee = NULL;
    char **line_split = my_str_to_word_array(line, " ");
    employee_t *tmp = employees;

    if (array_len(line_split) != 6)
        return (NULL);
    add_employee = malloc(sizeof(employee_t));
    if (add_employee == NULL)
        return (NULL);
    add_employee->id = atoi(line_split[5]);
    add_employee->last_name = strdup(line_split[1]);
    add_employee->name = strdup(line_split[2]);
    add_employee->job = strdup(line_split[3]);
    add_employee->zipcode = strdup(line_split[4]);
    add_employee->next = NULL;
    for (; tmp->next != NULL; tmp = tmp->next);
    tmp->next = add_employee;
    return (add_employee);
}

static employee_t *free_node(employee_t *employee)
{
    employee_t *tmp;

    free(employee->last_name);
    free(employee->name);
    free(employee->job);
    free(employee->zipcode);
    tmp = employee->next;
    free(employee);
    return (tmp);
}

employee_t *delete_employee(employee_t *employees, int id)
{
    employee_t *tmp = employees;

    if (tmp->id == id) {
        return free_node(tmp);
    }
    for (; tmp->next != NULL; tmp = tmp->next) {
            if (tmp->next->id == id) {
                tmp->next = free_node(tmp->next);
            }
            if (tmp->next == NULL)
                break;
    }
    return (employees);
}

employee_t *find_employee(employee_t *employees, int id)
{
    for (; employees != NULL; employees = employees->next) {
        if (employees->id == id)
            return (employees);
    }
    return (NULL);
}
