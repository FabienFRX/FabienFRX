##
## EPITECH PROJECT, 2023
## MAKEFILE
## File description:
## Makefile setting up
##

SRC     :=  src/main.c \
            src/manage_error.c \
            src/parsing/parsing1.c \
            src/parsing/parsing2.c \
            src/parsing/parsing3.c \
			src/parsing/parsing4.c \

OBJ     :=  $(SRC:.c=.o)

NAME    :=  amazed

CC      :=  gcc

LDFFLAGS := -lmy -L ./lib/my/

CFLAGS  :=  -Wall -Wextra

CPPFLAGS:=  -Iinclude

# COLORS
BLACK   :=  "\033[1;30m"
RED     :=  "\033[1;31m"
GREEN   :=  "\033[1;32m"
PURPLE  :=  "\033[1;35m"
CYAN    :=  "\033[1;36m"
WHITE   :=  "\033[1;37m"
BOLD    :=  "\033[1m"
UNDERLINE := "\033[4m"
RESET   :=  "\033[0m"

# Visual elements
BANNER  :=  $(PURPLE)$(BOLD)$(UNDERLINE)"*** SUPER MAKEFILE ***"$(RESET)
all:    $(NAME)

$(NAME):    $(OBJ)
	@echo $(BANNER)
	@echo $(CYAN)"Compiling $@...🛠️"$(RESET)
	@make -C lib/my -s
	@$(CC) $(SRC) -o $(NAME) $(CPPFLAGS) libmy.a $(CFLAGS)
	@echo $(GREEN)"✅ $@ compiled successfully!"$(RESET)

clean:
	@echo $(BANNER)
	@echo $(RED)"Cleaning..."$(RESET)
	@make -C lib/my/ clean -s
	@rm -f $(OBJ)
	@rm -f *~

fclean: clean
	@echo $(BANNER)
	@echo $(PURPLE)"Full Cleaning...🧹"$(RESET)
	@make -C lib/my/ fclean -s
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

# Loading bar
define loading_bar
	@echo -n ' '
	@echo -n '['
	@i=0; while [ $$i -lt $(1) ]; do \
		echo -n '==' ; \
		i=$$(($$i+1)) ; \
	done
	@echo -n '>'
	@i=0; while [ $$i -lt $$(($(2) - $(1))) ]; do \
		echo -n ' ' ; \
		i=$$(($$i+1)) ; \
	done
	@echo -n '] '
	@echo $(3)"/"$(2)
endef

$(OBJ): $(SRC)
	@$(call loading_bar,$(words $(OBJ)),$(words $(SRC)),"Compiling")
