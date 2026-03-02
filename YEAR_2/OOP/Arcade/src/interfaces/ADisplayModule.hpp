/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Mar, 2025                                                     *
 * Title           - B-OOP-400-TLS-4-1-arcade-nolan.dumoulin [WSL: Debian]              *
 * Description     -                                                                    *
 *     ADisplayModule                                                                   *
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

#include "../interfaces/IDisplayModule.hpp"
#include <string>

namespace arcade {
    class ADisplayModule : public IDisplayModule {
    public:
        ADisplayModule(const std::string &name, int width = 800, int height = 600);
        virtual ~ADisplayModule() = default;

        const std::string &getName() const override;
        bool isOpen() const override;
        int getWidth() const override;
        int getHeight() const override;

        virtual void init() override = 0;
        virtual void stop() override = 0;
        virtual void clear() override = 0;
        virtual void display() override = 0;
        virtual bool pollEvent(InputEvent &event) override = 0;
        virtual void drawRect(int x, int y, int width, int height, const Color &color) override = 0;
        virtual void drawCircle(int x, int y, int radius, const Color &color) override = 0;
        virtual void drawText(int x, int y, const std::string &text, const Color &color) override = 0;

    protected:
        std::string name;
        int width;
        int height;
        bool open;
    };
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