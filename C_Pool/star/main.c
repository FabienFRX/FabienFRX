void my_putchar(char c) {
	write(1, &c, 1);
}

int main(void) {
    star(0);
    star(1);
    star(5);
    star(2);
    return 0;
}
