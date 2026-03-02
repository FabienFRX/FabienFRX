##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Main Makefile
##

GREEN = \033[1;32m
YELLOW = \033[1;33m
CYAN = \033[1;36m
RESET = \033[0m

all: ia

server:
	@echo "$(CYAN)[SERVER] Build...$(RESET)"
	@$(MAKE) --no-print-directory -C server

gui:
	@echo "$(CYAN)[GUI] Build...$(RESET)"
	@$(MAKE) --no-print-directory -C gui

ia:
	@echo "$(CYAN)[IA] Build...$(RESET)"
	@$(MAKE) --no-print-directory -C ia

clean:
	@echo "$(YELLOW)Cleaning all...$(RESET)"
	@$(MAKE) --no-print-directory -C ia clean

fclean:
	@echo "$(YELLOW)Full clean all...$(RESET)"
	@$(MAKE) --no-print-directory -C ia fclean

re: fclean all

.PHONY: all server gui ia clean fclean re
