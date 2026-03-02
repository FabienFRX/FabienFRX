/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Mar, 2025                                                     *
 * Title           - B-OOP-400-TLS-4-1-arcade-nolan.dumoulin [WSL: Debian]              *
 * Description     -                                                                    *
 *     Game1                                                                            *
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

#include "../interfaces/AGameModule.hpp"
#include "../interfaces/IDisplayModule.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>

namespace arcade {
    struct Position {
        int x;
        int y;
        bool operator == (const Position &other) const {
            return x == other.x && y == other.y;
        }
    };

    class Snake : public AGameModule {
    public:
        Snake();
        ~Snake() override = default;

        void init() override;
        void stop() override;
        void update(float deltaTime) override;
        void render(IDisplayModule &displayModule) override;
        void handleEvent(const InputEvent &event) override;
        void reset() override;

    private:
        std::vector<Position> m_snake;
        Position m_food;
        Position m_direction;
        Position m_nextDirection;
        bool m_directionChanged;
        int m_width;
        int m_height;
        bool m_gameOver;
        int m_score;
        float moveTimer;
        float moveDelay;
        
        void moveSnake();
        void generateFood();
        bool checkCollision(const Position &pos) const;
        
        int calculateCellSize(const IDisplayModule &displayModule) const;
        int calculateOffsetX(const IDisplayModule &displayModule, int cellSize) const;
        int calculateOffsetY(const IDisplayModule &displayModule, int cellSize) const;
    };

    extern "C" {
        IGameModule *createGameModule();
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