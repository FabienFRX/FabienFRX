##
## EPITECH PROJECT, 2025
## B-YEP-400-TLS-4-1-zappy-fabien.fraixanet
## File description:
## Makefile for Zappy project
##

CXX = g++
CC = gcc

NAME_SERVER = zappy_server
NAME_GUI = zappy_gui
NAME_AI = zappy_ai

SRC_SERVER = server/parsing.c \
			server/server_utils.c \
			server/time_management.c \
			server/time_utils.c \
			server/ressource_management.c \
			server/connection_handler.c \
			server/player_creation.c \
			server/egg_management.c \
			server/basic_commands.c \
			server/look_command.c \
			server/ressource_commands.c \
			server/command_dispatcher.c \
			server/timed_executor.c \
			server/egg_utils.c \
			server/player_utils.c \
			server/incantation_commands.c \
			server/server_main.c \
			server/server_init.c \
			server/win_condition.c \
			server/player_movement.c \
			server/gui_communication.c \
			server/main.c

SRC_GUI = GUI/src/main.cpp \
          GUI/src/client.cpp \
          GUI/src/error_handling.cpp \
          GUI/src/init.cpp \
          GUI/src/map.cpp \
          GUI/src/Player.cpp \
          GUI/src/SpriteSheet.cpp

OBJ_SERVER = $(SRC_SERVER:.c=.o)
OBJ_GUI = $(SRC_GUI:.cpp=.o)

CPPFLAGS = -I./GUI/include -I./server/include -I./ai
CFLAGS = -W -Wall -Wextra -g3 -D_POSIX_C_SOURCE=200809L -Wno-unused-parameter -Wno-unused-function $(CPPFLAGS)
CXXFLAGS = -W -Wall -Wextra -g3 -D_POSIX_C_SOURCE=200809L -Wno-unused-parameter -Wno-unused-function $(CPPFLAGS)

LDLIBS_GUI = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
LDFLAGS = -lm

PY_FILES = ai/main.py

all: $(NAME_SERVER) $(NAME_GUI) $(NAME_AI)

$(NAME_SERVER): $(OBJ_SERVER)
	$(CC) -o $@ $^ $(LDFLAGS)

$(NAME_GUI): $(OBJ_GUI)
	$(CXX) -o $@ $^ $(LDLIBS_GUI)

$(NAME_AI): $(PY_FILES)
	@echo "#!/bin/bash" > $(NAME_AI)
	@echo "SCRIPT_DIR=\"\$$(cd \"\$$(dirname \"\$$0\")\"; pwd)\"" >> $(NAME_AI)
	@echo "python3 \"\$$SCRIPT_DIR/ai/main.py\" \"\$$@\"" >> $(NAME_AI)
	@chmod +x $(NAME_AI)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run_ai:
	python3 ai/main.py

clean:
	rm -f $(OBJ_SERVER) $(OBJ_GUI)

fclean: clean
	rm -f $(NAME_SERVER) $(NAME_GUI) $(NAME_AI)

re: fclean all

.PHONY: all clean fclean re run_ai $(NAME_AI)