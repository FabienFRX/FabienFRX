# 🎮 Arcade

```ascii
           _____   _____          _____  ______ 
     /\   |  __ \ / ____|   /\   |  __ \|  ____|
    /  \  | |__) | |       /  \  | |  | | |__
   / /\ \ |  _  /| |      / /\ \ | |  | |  __|
  / ____ \| | \ \| |____ / ____ \| |__| | |____
 /_/    \_\_|  \_\\_____/_/    \_\_____/|______|
```

## 📝 Description

A retro gaming platform that lets the user choose a game to play and keeps track of player scores. The program loads graphics libraries and games dynamically at runtime.

## ✨ Features

- Dynamic loading of graphics libraries and games at runtime
- Switching between graphics libraries without restarting the program
- Switching between games during execution
- High score tracking
- Menu system for selecting games and display libraries

## 🎨 Graphics Libraries

- **NCurses** - Text-based terminal graphics with mouse support
- **SDL2** - 2D graphics with hardware acceleration
- **SFML** - Simple and Fast Multimedia Library with enhanced drawing capabilities

## 🎲 Games

- **Snake** - Classic snake game where you grow by eating food and avoid collisions
- **Minesweeper** - Logic puzzle where you must clear a minefield without detonating any mines

## 🔧 Building and Compiling

To build your new libraries, use the provided build system:

### 🛠️ Using Makefile:

```bash
# Build everything
make

# Build just the core
make core

# Build just the graphics libraries
make graphicals

# Build just the games
make games
```

The compiled libraries will be placed in the `./lib/` directory.

### 📋 Prerequisites

You'll need the following libraries installed:
- libncurses-dev
- libsdl2-dev
- libsdl2-ttf-dev
- libsfml-dev

On Debian/Ubuntu:
```
sudo apt install libncurses-dev libsdl2-dev libsdl2-ttf-dev libsfml-dev
```

On Fedora:
```
sudo dnf install ncurses-devel SDL2-devel SDL2_ttf-devel SFML-devel
```

### 🚀 Running the program

```
./arcade ./lib/arcade_ncurses.so
```
or
```
./arcade ./lib/arcade_sdl2.so
```
or
```
./arcade ./lib/arcade_sfml.so
```

## 🕹️ Controls

- **Tab** - Switch to the next graphics library
- **LShift** - Switch to the next game
- **R** - Restart the current game
- **ESC** - Return to menu (in game) or exit (in menu)
- **Arrow Keys** - Navigation in games
- **Mouse** - Selection in menu and some game actions
- **Enter** - Start a game from the menu

## 📁 Project Structure

- `src/core` - Core program files
- `src/graphics` - Graphics library implementations
- `src/games` - Game implementations
- `src/interfaces` - Interface definitions
- `src/exceptions` - Exception classes
- `src/utils` - Utility classes
- `lib/` - Compiled dynamic libraries
- `doc/` - Documentation

## 📚 Documentation

Implementation guides and class diagrams are available in the `doc/` directory.

## 🤝 Interface Collaboration

We have collaborated with our colleagues for the interface design.

Contact information for the group leader we collaborated with:
- Email: [Tony Fournales](https://github.com/prestigetonio) tony.fournales@epitech.eu

## ⚖️ License

MIT License - see LICENSE file for details.