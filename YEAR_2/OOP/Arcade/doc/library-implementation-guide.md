# Arcade Library Implementation Guide

## Overview
The Arcade project is designed to be a highly extensible gaming platform that allows dynamic loading of graphics libraries and game modules at runtime.

## Implementing a Graphics Library

### Interface Requirements
To create a new graphics library, you must implement the `IDisplayModule` interface defined in `IDisplayModule.hpp`:

```cpp
class IDisplayModule {
public:
    virtual void init() = 0;
    virtual void stop() = 0;
    virtual const std::string &getName() const = 0;
    virtual void clear() = 0;
    virtual void display() = 0;
    virtual bool isOpen() const = 0;

    // Event handling
    virtual bool pollEvent(InputEvent &event) = 0;

    // Drawing methods
    virtual void drawRect(int x, int y, int width, int height, const Color &color) = 0;
    virtual void drawCircle(int x, int y, int radius, const Color &color) = 0;
    virtual void drawText(int x, int y, const std::string &text, const Color &color) = 0;

    // Window dimensions
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
};
```

### Entry Point Requirement
Each graphics library must provide a C-style entry point function:

```cpp
extern "C" {
    IDisplayModule *createDisplayModule() {
        return new YourDisplayModule();
    }
}
```

### Implementing Key Methods

#### Initialization
- `init()`: Set up the display window, initialize resources
- `stop()`: Clean up resources, close the window

#### Event Handling
- `pollEvent()`: Process input events (keyboard, mouse)
- Translate library-specific events to `InputEvent` structure

#### Drawing Methods
- Implement drawing primitives (rectangles, text, etc...)
- Support different color rendering

## Implementing a Game Library

### Interface Requirements
Create a game module implementing the `IGameModule` interface:

```cpp
class IGameModule {
public:
    virtual void init() = 0;
    virtual void stop() = 0;
    virtual const std::string &getName() const = 0;

    // Game loop methods
    virtual void update(float deltaTime) = 0;
    virtual void render(IDisplayModule &displayModule) = 0;
    virtual void handleEvent(const InputEvent &event) = 0;

    virtual bool isGameOver() const = 0;
    virtual int getScore() const = 0;
    virtual void reset() = 0;
};
```

### Entry Point Requirement
Similar to graphics libraries:

```cpp
extern "C" {
    IGameModule *createGameModule() {
        return new YourGameModule();
    }
}
```

### Key Implementation Details
- `update()`: Advance game state based on time
- `render()`: Draw game state using the display module
- `handleEvent()`: Process user input
- `isGameOver()`: Check game completion state
- `reset()`: Restart the game to initial state

## Compilation Requirements
- Compile as a shared library (`.so`)
- Use `-fno-gnu-unique` compilation flag
- Place libraries in the `./lib/` directory

## Naming Convention
- Game libraries: `arcade_<gamename>.so`
  Example: `arcade_snake.so`
- Graphics libraries: `arcade_<libraryname>.so`
  Example: `arcade_sdl2.so`

## Best Practices
1. Keep game logic independent of rendering
2. Use standard input event types
3. Handle different screen sizes gracefully
4. Implement clean resource management
5. Provide clear, consistent game mechanics
```