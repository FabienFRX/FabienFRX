##
## EPITECH PROJECT, 2025
## B-PDG-300-TLS-3-1-PDGRUSH3-sven.reichert
## File description:
## Makefile
##

SRC	=	src/main.cpp	\
		src/CpuModule.cpp	\
		src/MonitorCore.cpp	\
		src/IModule.cpp	\
		src/IDisplay.cpp	\
		src/DateTimeModule.cpp	\
		src/HostnameModule.cpp	\
		src/NcursesDisplay.cpp	\
		src/OsModule.cpp	\
		src/RamModule.cpp	\
		src/SfmlDisplay.cpp	\

OBJ	=	$(SRC:.c=.o)

NAME	=	MyGKrellm

CFLAGS =	-std=c++20 -Wall -Wextra -Werror -I./include

CC =	g++

all:	$(NAME)

$(NAME):	$(OBJ)
	$(CC) -o $(NAME) $(OBJ) -lncurses -lsfml-graphics -lsfml-window -lsfml-system
clean:
	rm -f $(OBJ)

fclean:	clean
	rm -rf $(NAME)

re:	fclean all

.PHONY:	all clean fclean re