/*
** EPITECH PROJECT, 2024
** B-CPE-200-TLS-2-1-robotfactory-fabien.fraixanet [WSL: Debian]
** File description:
** main
*/

#include "../include/asm.h"

int my_asm(testa_t *head)
{
    if (head->data == NULL)
        return 84;
    head->data = get_filtered_text(head);
    parse_name(head);
    if (head->status == 84)
        return 84;
    parse_comment(head);
    if (head->status == 84)
        return 84;
    convert_in_cor(head);
    return 0;
}

int main(int argc, char *argv[])
{
    testa_t head;

    if (argc == 2 && my_strcmp(argv[1], "-h") == 0)
        return display_help();
    if (robustness(argc, argv) == 84)
        return 84;
    initialize_header(&head, argv);
    return (my_asm(&head));
}
