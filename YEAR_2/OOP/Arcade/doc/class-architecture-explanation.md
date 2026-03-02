# Arcade Class Architecture and Diagram Explanation

## Core Architecture Overview

### Key Interfaces
1. **IDisplayModule**: Defines graphics rendering and event handling
2. **IGameModule**: Defines game logic and rendering interface
3. **ADisplayModule**: Abstract base class for display implementations
4. **AGameModule**: Abstract base class for game implementations

### Core Classes
- **Core**: Central management class
  - Handles library loading
  - Manages game and display module switching
  - Handles menu and game state
  - Manages high scores

- **DynamicLoader**: Generic dynamic library loading mechanism
  - Uses `dlopen`, `dlsym` for runtime library loading
  - Provides type-safe library instance creation

### Relationship Diagram

```
Interfaces:
    ┌───────────────┐         ┌───────────────┐
    │ IDisplayModule│         │  IGameModule  │
    └───────┬───────┘         └───────┬───────┘
            │                         │
            ▼                         ▼
    ┌───────────────┐         ┌───────────────┐
    │ADisplayModule │         │ AGameModule   │
    └───────┬───────┘         └───────┬───────┘
            │                         │
            ▼                         ▼
    Concrete Implementations:   Concrete Games:
    - NCursesModule             - Snake
    - SDLModule                 - Minesweeper
    - SFMLModule                

Core Class Relationships:
    ┌───────────────┐
    │     Core      │
    └───────┬───────┘
            │
    Uses    │
┌───────────┴───────────┐
│    DynamicLoader      │
└─────────┬─────────────┘
          │
    Loads │
┌─────────┴─────────────┐
│    Display/Game       │
│     Libraries         │
└───────────────────────┘
```

## Core Workflow

1. **Initialization**
   - Scan available libraries in `./lib/`
   - Load initial display library
   - Load high scores

2. **Main Loop**
   - Process input events
   - Update game state
   - Render current view (menu or game)

3. **Library Management**
   - Dynamic library loading
   - Runtime module switching
   - Maintain library independence

## Key Design Principles

- **Polymorphism**: Interfaces allow runtime polymorphic behavior
- **Dependency Injection**: Display module injected into game module
- **Single Responsibility**: Clear separation of concerns
- **Open/Closed Principle**: Easily extensible without modifying core

## Dynamic Loading Mechanism

```cpp
template <typename T>
class DynamicLoader {
public:
    T* getInstance(const std::string &entryPoint) {
        void *handle = dlopen(libraryPath.c_str(), RTLD_LAZY);
        using CreateFunc = T *(*)();
        CreateFunc createInstance = 
            reinterpret_cast<CreateFunc>(dlsym(handle, entryPoint.c_str()));
        return createInstance();
    }
};
```

## Event Handling Flow

1. Display module polls events
2. Events translated to `InputEvent`
3. Core routes events to current game module
4. Game module processes relevant events

## Performance and Flexibility Considerations

- Minimal overhead in library switching
- Standard interface allows seamless module replacement
- Support for diverse rendering and input methods
