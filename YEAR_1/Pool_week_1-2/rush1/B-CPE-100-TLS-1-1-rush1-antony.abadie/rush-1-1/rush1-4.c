/*
d** EPITECH PROJECT, 2023
** rush1-4
** File description:
** display square with 'B' as 'B' and 'A' on first y and 'C' on last y
*/

/*
** EPITECH PROJECT, 2023
** rush2
** File description:
** display square with '/'
*/

void write_1_4(int x, int y, int nx, int ny)
{
    if ((nx == 1 && ny == 1) || (nx == 1 && ny == y)) {
        my_putchar('A');
    }
    if ((nx == x && ny == y) || (nx == x && ny == 1)) {
        my_putchar('C');
    }
}

void write_2_4(int x, int y, int nx, int ny)
{
    if ((nx == 1 || nx == x) && (ny > 1 && ny < y)) {
        my_putchar('B');
    }
    if ((ny == 1 || ny == y) && (nx > 1 && nx < x)) {
        my_putchar('B');
    }
}

void write_3_4(int x, int y, int nx, int ny)
{
    if (nx > 1 && ny != 1 && ny < y) {
        my_putchar(' ');
    }
}
void start_1_4(int x, int y)
{
    for (int ny = 1; ny <= y; ny++) {
        for (int nx = 1; nx <= x; nx++) {
            write_1_4(x, y, nx, ny);
            write_2_4(x, y, nx, ny);
            write_3_4(x, y, nx, ny);
        }
        my_putchar('\n');
    }
}

void rush_invalid_4(int x, int y)
{
    if (x <= 0 || y <= 0) {
        write(2, "Invalid size\n", 13);
    } else {
        start_rush(x , y);
    }
}
