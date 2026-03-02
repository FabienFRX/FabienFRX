/*
** EPITECH PROJECT, 2024
** B-SYN-200-TLS-2-1-calendar-fabien.fraixanet
** File description:
** manage_meeting
*/

#include "syn.h"

int array_len(char **arr)
{
    int i = 0;

    for (; arr[i] != NULL; i++);
    return i;
}

meeting_t *init_meeting(char *line)
{
    meeting_t *meetings = NULL;
    char **line_split = my_str_to_word_array(line, " ");

    if (line_split == NULL || array_len(line_split) < 5)
        return (NULL);
    meetings = malloc(sizeof(meeting_t));
    if (meetings == NULL)
        return (NULL);
    meetings->zipcode = strdup(line_split[1]);
    meetings->day = strdup(line_split[2]);
    meetings->id = atoi(line_split[3]);
    meetings->size = array_len(line_split) - 3;
    meetings->employees_id = malloc(sizeof(int) * (meetings->size + 1));
    for (int i = 4; line_split[i] != NULL; i++)
        meetings->employees_id[i - 4] = atoi(line_split[i]);
    meetings->employees_id[meetings->size] = -1;
    meetings->next = NULL;
    return (meetings);
}

meeting_t *add_meeting(meeting_t *meetings, char *line)
{
    meeting_t *add_meeting = NULL;
    char **line_split = my_str_to_word_array(line, " ");

    add_meeting = malloc(sizeof(meeting_t));
    if (line_split == NULL || array_len(line_split) < 5 || add_meeting == NULL)
        return (NULL);
    add_meeting->zipcode = strdup(line_split[1]);
    add_meeting->day = strdup(line_split[2]);
    add_meeting->id = atoi(line_split[3]);
    meetings->size = array_len(line_split) - 4;
    add_meeting->employees_id = malloc(sizeof(int) * (meetings->size + 1));
    for (int i = 4; line_split[i] != NULL; i++)
        add_meeting->employees_id[i - 4] = atoi(line_split[i]);
    add_meeting->employees_id[meetings->size] = -1;
    add_meeting->next = NULL;
    for (; meetings->next != NULL; meetings = meetings->next);
    meetings->next = add_meeting;
    return (add_meeting);
}

static meeting_t *free_node(meeting_t *meetings)
{
    meeting_t *tmp;

    free(meetings->zipcode);
    free(meetings->day);
    free(meetings->employees_id);
    tmp = meetings->next;
    free(meetings);
    return (tmp);
}

meeting_t *delete_meeting(meeting_t *meetings, int id)
{
    if (meetings->id == id) {
        return free_node(meetings);
    }
    for (; meetings->next != NULL; meetings = meetings->next) {
            if (meetings->next->id == id) {
                meetings->next = free_node(meetings->next);
            }
    }
    return (meetings);
}

meeting_t *find_meeting(meeting_t *meetings, int id)
{
    for (; meetings != NULL; meetings = meetings->next) {
        if (meetings->id == id)
            return (meetings);
    }
    return (NULL);
}
