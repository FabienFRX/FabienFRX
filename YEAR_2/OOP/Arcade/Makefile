##
## EPITECH PROJECT, 2025
## Arcade
## File description:
## Makefile
##

# Compiler and compiler flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -fno-gnu-unique -fPIC -g -I$(SRC_DIR)
LDFLAGS = -shared -ldl

# Define directories
SRC_DIR = src
BUILD_DIR = build
LIB_DIR = ./lib/

# Core program
CORE_SRCS = $(SRC_DIR)/core/Arcade.cpp \
            $(SRC_DIR)/core/Core.cpp \
            $(SRC_DIR)/interfaces/ADisplayModule.cpp \
            $(SRC_DIR)/interfaces/AGameModule.cpp \
            $(SRC_DIR)/utils/LibraryScanner.cpp
CORE_OBJS = $(CORE_SRCS:%.cpp=$(BUILD_DIR)/%.o)
CORE_TARGET = arcade

# Graphic libraries
NCURSES_SRCS = $(SRC_DIR)/graphics/NCursesModule.cpp
NCURSES_OBJS = $(NCURSES_SRCS:%.cpp=$(BUILD_DIR)/%.o)
NCURSES_TARGET = $(LIB_DIR)/arcade_ncurses.so
NCURSES_LIBS = -lncurses

SDL_SRCS = $(SRC_DIR)/graphics/SDLModule.cpp
SDL_OBJS = $(SDL_SRCS:%.cpp=$(BUILD_DIR)/%.o)
SDL_TARGET = $(LIB_DIR)/arcade_sdl2.so
SDL_LIBS = -lSDL2 -lSDL2_ttf

SFML_SRCS = $(SRC_DIR)/graphics/SFMLModule.cpp
SFML_OBJS = $(SFML_SRCS:%.cpp=$(BUILD_DIR)/%.o)
SFML_TARGET = $(LIB_DIR)/arcade_sfml.so
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Game libraries
SNAKE_SRCS = $(SRC_DIR)/games/Snake.cpp
SNAKE_OBJS = $(SNAKE_SRCS:%.cpp=$(BUILD_DIR)/%.o)
SNAKE_TARGET = $(LIB_DIR)/arcade_snake.so

MINESWEEPER_SRCS = $(SRC_DIR)/games/Minesweeper.cpp
MINESWEEPER_OBJS = $(MINESWEEPER_SRCS:%.cpp=$(BUILD_DIR)/%.o)
MINESWEEPER_TARGET = $(LIB_DIR)/arcade_minesweeper.so

# Define all targets
GRAPHICAL_TARGETS = $(NCURSES_TARGET) $(SDL_TARGET) $(SFML_TARGET)
GAME_TARGETS = $(SNAKE_TARGET) $(MINESWEEPER_TARGET)
ALL_TARGETS = $(CORE_TARGET) $(GRAPHICAL_TARGETS) $(GAME_TARGETS)

# Default rule
all: directories $(ALL_TARGETS)

# Create necessary directories
directories:
	@mkdir -p $(BUILD_DIR)/$(SRC_DIR)/core
	@mkdir -p $(BUILD_DIR)/$(SRC_DIR)/graphics
	@mkdir -p $(BUILD_DIR)/$(SRC_DIR)/games
	@mkdir -p $(BUILD_DIR)/$(SRC_DIR)/interfaces
	@mkdir -p $(BUILD_DIR)/$(SRC_DIR)/utils
	@mkdir -p $(LIB_DIR)

# Core program
$(CORE_TARGET): $(CORE_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -ldl -pthread

# Graphic libraries
$(NCURSES_TARGET): $(NCURSES_OBJS) $(BUILD_DIR)/$(SRC_DIR)/interfaces/ADisplayModule.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(NCURSES_LIBS)

$(SDL_TARGET): $(SDL_OBJS) $(BUILD_DIR)/$(SRC_DIR)/interfaces/ADisplayModule.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(SDL_LIBS)

$(SFML_TARGET): $(SFML_OBJS) $(BUILD_DIR)/$(SRC_DIR)/interfaces/ADisplayModule.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(SFML_LIBS)

# Game libraries
$(SNAKE_TARGET): $(SNAKE_OBJS) $(BUILD_DIR)/$(SRC_DIR)/interfaces/AGameModule.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ 

$(MINESWEEPER_TARGET): $(MINESWEEPER_OBJS) $(BUILD_DIR)/$(SRC_DIR)/interfaces/AGameModule.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

# Compile all source files to object files
$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Specific targets required by the project
core: directories $(CORE_TARGET)

graphicals: directories $(GRAPHICAL_TARGETS)

games: directories $(GAME_TARGETS)

# Clean rules
clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(CORE_TARGET)
	rm -rf $(LIB_DIR)

re: fclean all

# Phony targets
.PHONY: all directories core graphicals games clean fclean re