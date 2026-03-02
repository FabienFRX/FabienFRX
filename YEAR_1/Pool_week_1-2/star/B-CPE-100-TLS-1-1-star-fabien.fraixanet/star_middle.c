/*
** EPITECH PROJECT, 2023
** star_middle
** File description:
** function for middle in star
*/

void draw_middle_line(int size, int move_side)
{
    int total = (size * 2) + 1 + (size * 2) - 3 + (size * 2) + 1 - move_side;

    for (int i = 1; i <= move_side; i++) {
        my_putchar(' ');
    }
    my_putchar('*');
    for (int j = 1; j <= total - (move_side + 2); j++) {
        my_putchar(' ');
    }
    my_putchar('*');
    my_putchar('\n');
}

void draw_middle(int size)
{
    for (int i = 1; i <= size; i++) {
        draw_middle_line(size, i);
    }
    for ( int j = 1; j < size; j++) {
        draw_middle_line(size, size - j);
    }
}
