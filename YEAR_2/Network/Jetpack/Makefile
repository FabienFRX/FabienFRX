##
## EPITECH PROJECT, 2025
## Jetpack
## File description:
## Makefile
##

## -------- COMPIL ----##

CXX     = g++

## -------- SRC -------##

SRC_SERVER	=	$(shell find ./src/server -name "*.cpp")	\
				src/client/Client.cpp	\
				src/error/Error.cpp		\

SRC_CLIENT	=	$(shell find ./src/client -name "*.cpp")	\
				src/error/Error.cpp	\

## -------- OBJ -------##

OBJ_SERVER = $(SRC_SERVER:.cpp=.o)
OBJ_CLIENT = $(SRC_CLIENT:.cpp=.o)

## ------- FLAGS --------##

CPPFLAGS        =	-std=c++20 -Wall -Wextra -Iincludes/ -fPIC

## ------- BIN ----------##

NAME_SERVER       =	jetpack_server
NAME_CLIENT       =	jetpack_client

## --------- RULES --------##

all:	server client

server:	$(NAME_SERVER)

client:	$(NAME_CLIENT)

$(NAME_SERVER)	:	$(OBJ_SERVER)
	$(CXX) -o $(NAME_SERVER) $(OBJ_SERVER) $(CPPFLAGS)

$(NAME_CLIENT)	:	$(OBJ_CLIENT)
	$(CXX) -o $(NAME_CLIENT) $(OBJ_CLIENT) $(CPPFLAGS)

clean:
	rm -rf $(shell find . -name "*.o")
	rm -rf *# *~

fclean: clean
	rm -f $(NAME_SERVER)
	rm -f $(NAME_CLIENT)

re:	fclean all

.PHONY	:	all clean fclean re