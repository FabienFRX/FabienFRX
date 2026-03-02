/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Mar, 2025                                                     *
 * Title           - B-OOP-400-TLS-4-1-arcade-nolan.dumoulin [WSL: Debian]              *
 * Description     -                                                                    *
 *     SDLModule                                                                        *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _|           *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _|           *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_|           *
 *       _|        _|          _|        _|      _|        _|        _|    _|           *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _|           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#pragma once

#include "../interfaces/ADisplayModule.hpp"
#include "../exceptions/DisplayException.hpp"
#include <cmath>
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>

namespace arcade {
    class SDLModule : public ADisplayModule {
    private:
        SDL_Window *window;
        SDL_Renderer *renderer;
        TTF_Font *font;
        bool initSDL();
        bool initTTF();
        SDL_Color convertColor(const Color &color);

    public:
        SDLModule();
        ~SDLModule() override;

        void init() override;
        void stop() override;
        void clear() override;
        void display() override;
        bool pollEvent(InputEvent &event) override;
        void drawRect(int x, int y, int width, int height, const Color &color) override;
        void drawCircle(int x, int y, int radius, const Color &color) override;
        void drawText(int x, int y, const std::string &text, const Color &color) override;
    };

    extern "C" {
        IDisplayModule *createDisplayModule();
    }
}

/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * MIT License                                                                          *
 * Copyright (c) 2025 Anonymous                                                         *
 *                                                                                      *
 * Permission is hereby granted, free of charge, to any person obtaining a copy         *
 * of this software and associated documentation files (the "Software"), to deal        *
 * in the Software without restriction, including without limitation the rights         *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell            *
 * copies of the Software, and to permit persons to whom the Software is                *
 * furnished to do so, subject to the following conditions:                             *
 *                                                                                      *
 * The above copyright notice and this permission notice shall be included in all       *
 * copies or substantial portions of the Software.                                      *
 *                                                                                      *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR           *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,             *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE          *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER               *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,        *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE        *
 * SOFTWARE.                                                                            *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */