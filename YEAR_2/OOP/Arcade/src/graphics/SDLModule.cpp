/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Mar, 2025                                                     *
 * Title           - B-OOP-400-TLS-4-1-arcade-nolan.dumoulin [WSL: Debian]              *
 * Description     -                                                                    *
 *     SDLModule                                                                        *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░             *
 *         ▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓             *
 *         █  ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "SDLModule.hpp"

namespace arcade {
    SDLModule::SDLModule() : ADisplayModule("SDL2", 800, 600), window(nullptr), renderer(nullptr), font(nullptr) {}

    SDLModule::~SDLModule()
    {
        stop();
    }

    void SDLModule::init()
    {
        if (!initSDL()) {
            throw arcade::DisplayException("Failed to initialize SDL");
        }
        if (!initTTF()) {
            throw arcade::DisplayException("Failed to initialize SDL_TTF");
        }
        open = true;
    }

    bool SDLModule::initSDL()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw arcade::DisplayException(SDL_GetError());
        }

        window = SDL_CreateWindow("Arcade", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
        if (!window) {
            throw arcade::DisplayException("Failed to create window: " + std::string(SDL_GetError()));
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            throw arcade::DisplayException("Failed to create renderer: " + std::string(SDL_GetError()));
        }

        return true;
    }

    bool SDLModule::initTTF()
    {
        if (TTF_Init() < 0) {
            throw arcade::DisplayException("Failed to initialize TTF: " + std::string(TTF_GetError()));
        }

        const std::vector<std::string> fontPaths = {
            "./assets/fonts/DejaVuSansCondensed.ttf",
            "assets/fonts/DejaVuSansCondensed.ttf",
            "/usr/share/fonts/truetype/dejavu/DejaVuSansCondensed.ttf",
            "/usr/share/fonts/TTF/DejaVuSans.ttf",
            "/usr/share/fonts/dejavu/DejaVuSans.ttf"
        };

        for (const auto &path : fontPaths) {
            font = TTF_OpenFont(path.c_str(), 18);
            if (font) {
                break;
            }
        }

        if (!font) {
            std::cerr << "Warning: Failed to load any font: " << TTF_GetError() << std::endl;
            std::cerr << "Text rendering will be disabled." << std::endl;
            return true;
        }

        return true;
    }

    void SDLModule::stop()
    {
        if (open) {
            if (font) {
                TTF_CloseFont(font);
                font = nullptr;
            }
            if (renderer) {
                SDL_DestroyRenderer(renderer);
                renderer = nullptr;
            }
            if (window) {
                SDL_DestroyWindow(window);
                window = nullptr;
            }
            TTF_Quit();
            SDL_Quit();
            open = false;
        }
    }

    void SDLModule::clear()
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    void SDLModule::display()
    {
        SDL_RenderPresent(renderer);
    }

    bool SDLModule::pollEvent(InputEvent &event)
    {
        SDL_Event sdlEvent;

        if (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    event.type = EventType::WindowClosed;
                    return true;

                case SDL_KEYDOWN:
                    event.type = EventType::KeyPressed;

                    switch (sdlEvent.key.keysym.sym) {
                        case SDLK_UP:
                            event.key.keyCode = KeyCode::Up;
                            break;
                        case SDLK_DOWN:
                            event.key.keyCode = KeyCode::Down;
                            break;
                        case SDLK_LEFT:
                            event.key.keyCode = KeyCode::Left;
                            break;
                        case SDLK_RIGHT:
                            event.key.keyCode = KeyCode::Right;
                            break;
                        case SDLK_RETURN:
                            event.key.keyCode = KeyCode::Enter;
                            break;
                        case SDLK_ESCAPE:
                            event.key.keyCode = KeyCode::Escape;
                            break;
                        case SDLK_SPACE:
                            event.key.keyCode = KeyCode::Space;
                            break;
                        case SDLK_TAB:
                            event.key.keyCode = KeyCode::Tab;
                            break;
                        case SDLK_BACKSPACE:
                            event.key.keyCode = KeyCode::Backspace;
                            break;
                        case SDLK_LSHIFT:
                            event.key.keyCode = KeyCode::Shift;
                            break;
                        default:
                            if (sdlEvent.key.keysym.sym >= SDLK_a && sdlEvent.key.keysym.sym <= SDLK_z) {
                                event.key.keyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::A) + (sdlEvent.key.keysym.sym - SDLK_a));
                            } else if (sdlEvent.key.keysym.sym >= SDLK_0 && sdlEvent.key.keysym.sym <= SDLK_9) {
                                event.key.keyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::Num0) + (sdlEvent.key.keysym.sym - SDLK_0));
                            } else {
                                event.key.keyCode = KeyCode::Unknown;
                            }
                            break;
                    }
                    return true;

                case SDL_MOUSEBUTTONDOWN:
                event.type = EventType::MouseButtonPressed;
                event.mouseButton.x = sdlEvent.button.x;
                event.mouseButton.y = sdlEvent.button.y;
                switch (sdlEvent.button.button) {
                    case SDL_BUTTON_LEFT:
                        event.mouseButton.button = 0; // Clic gauche
                        break;
                    case SDL_BUTTON_RIGHT:
                        event.mouseButton.button = 1; // Clic droit
                        break;
                    case SDL_BUTTON_MIDDLE:
                        event.mouseButton.button = 2; // Clic du milieu
                        break;
                    default:
                        event.mouseButton.button = -1; // Bouton inconnu
                        break;
                }
                return true;

                case SDL_MOUSEMOTION:
                    event.type = EventType::MouseMoved;
                    event.mouseMove.x = sdlEvent.motion.x;
                    event.mouseMove.y = sdlEvent.motion.y;
                    return true;
            }
        }

        return false;
    }

    SDL_Color SDLModule::convertColor(const Color &color)
    {
        return {color.r, color.g, color.b, color.a};
    }

    void SDLModule::drawRect(int x, int y, int width, int height, const Color &color)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

        SDL_Rect rect = {x, y, width, height};
        SDL_RenderFillRect(renderer, &rect);
    }

    void SDLModule::drawCircle(int x, int y, int radius, const Color &color)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

        for (int w = -radius; w <= radius; w++) {
            for (int h = -radius; h <= radius; h++) {
                if (w*w + h*h <= radius*radius) {
                    SDL_RenderDrawPoint(renderer, x + w, y + h);
                }
            }
        }
    }

    void SDLModule::drawText(int x, int y, const std::string &text, const Color &color)
    {
        SDL_Color sdlColor = convertColor(color);
        SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), sdlColor);
        if (!surface) {
            return;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            SDL_FreeSurface(surface);
            return;
        }

        SDL_Rect dstRect = {x, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

    extern "C" {
        IDisplayModule *createDisplayModule() {
            return new SDLModule();
        }
    }
}
