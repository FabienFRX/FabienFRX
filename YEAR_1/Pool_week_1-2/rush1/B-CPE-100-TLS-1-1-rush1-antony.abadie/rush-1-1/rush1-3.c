/*
** EPITECH PROJECT, 2023
** rush1-3
** File description:
** display square with A B C
*/

void write_1_3(int x, int y, int nx, int ny)
{
    if ((nx == 1 && ny == 1) || (nx == x && ny == 1)){
        my_putchar('A');
    }
    if ((nx == 1 && ny == y) || (nx == x && ny == y)) {
        my_putchar('C');
    }
}

void write_2_3(int x, int y, int nx, int ny)
{
    if ((nx == 1 || nx == x) && (ny > 1 && ny < y)) {
        my_putchar('B');
    }
    if ((ny == 1 || ny == y) && (nx > 1 && nx < x)) {
        my_putchar('B');
    }
}

void write_3_3(int x, int y, int nx, int ny)
{
    if (nx > 1 && ny != 1 && ny < y) {
        my_putchar(' ');
    }
}

void start_3(int x, int y)
{
    for (int ny = 1; ny <= y; ny++) {
        for (int nx = 1; nx <= x; nx++) {
            write_1_3(x, y, nx, ny);
            write_2_3(x, y, nx, ny);
            write_3_3(x, y, nx, ny);
        }
        my_putchar('\n');
    }
}

void rush_invalid_3(int x, int y)
{
    if (x <= 0 || y <= 0) {
        write(2, "Invalid size\n", 13);
    } else {
        start_rush(x , y);
    }
}
