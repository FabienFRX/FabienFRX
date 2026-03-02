CC = gcc
CFLAGS = -Wall -Wextra -Werror
TARGET = app
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

re: clean all

.PHONY: all clean re
