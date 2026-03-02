/*
** EPITECH PROJECT, 2023
** rush2
** File description:
** display square with '/'
*/

void write_1(int x, int y, int nx, int ny)
{
    if ((nx == 1 && ny == 1) || (nx == x && ny == y)) {
        my_putchar('/');
    }
    if ((nx == 1 && ny == y) || (nx == x && ny == 1)) {
        my_putchar('\\');
    }
}

void write_2(int x, int y, int nx, int ny)
{
    if ((nx == 1 || nx == x) && (ny > 1 && ny < y)) {
        my_putchar('*');
    }
    if ((ny == 1 || ny == y) && (nx > 1 && nx < x)) {
        my_putchar('*');
    }
}

void write_3(int x, int y, int nx, int ny)
{
    if (nx > 1 && ny != 1 && ny < y) {
        my_putchar(' ');
    }
}

void start_1(int x, int y)
{
    for (int ny = 1; ny <= y; ny++) {
        for (int nx = 1; nx <= x; nx++) {
            write_1(x, y, nx, ny);
            write_2(x, y, nx, ny);
            write_3(x, y, nx, ny);
        }
        my_putchar('\n');
    }
}

void rush_invalid(int x, int y)
{
    if (x <= 0 || y <= 0) {
        write(2, "Invalid size\n", 13);
    } else {
        start_rush(x , y);
    }
}
