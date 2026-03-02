/*
** EPITECH PROJECT, 2023
** rush1
** File description:
** display square
*/

void write_rush(int x, int y, int nx, int ny)
{
    if ((nx == 1 || nx == x) && (ny == 1 || ny == y)) {
        my_putchar('o');
    }
    if ((ny == 1 || ny == y) && (nx > 1 && nx < x)) {
        my_putchar('-');
    }
}

void write_rush2(int x, int y, int nx, int ny)
{
    if ((nx == 1 || nx == x) && (ny > 1 && ny < y)) {
        my_putchar('|');
    }
    if (nx > 1 && ny != 1 && ny < y) {
        my_putchar(' ');
    }
}

void start_rush(int x, int y)
{
    for (int ny = 1; ny <= y; ny++) {
        for (int nx = 1; nx <= x; nx++) {
            write_rush(x, y, nx, ny);
            write_rush2(x, y, nx, ny);
        }
        my_putchar('\n');
    }
}

void rush(int x, int y)
{
    if (x <= 0 || y <= 0) {
        write(2, "Invalid size\n", 13);
    } else {
        start_rush(x , y);
    }
}
