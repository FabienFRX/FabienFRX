## Makefile pour le projet Gomoku AI

NAME    = pbrain-gomoku-ai

CC      = g++
CFLAGS  = -W -Wall -Wextra -Werror -std=c++17

SRC     = src/Main.cpp \
          src/CommandParser.cpp \
          src/GomokuEngine.cpp

OBJ     = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re