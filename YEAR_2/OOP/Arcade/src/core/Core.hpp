/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Mar, 2025                                                     *
 * Title           - B-OOP-400-TLS-4-1-arcade-nolan.dumoulin [WSL: Debian]              *
 * Description     -                                                                    *
 *     ACore                                                                             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#pragma once

#include "../interfaces/IDisplayModule.hpp"
#include "../interfaces/IGameModule.hpp"
#include "../utils/DynamicLoader.hpp"
#include "../utils/LibraryScanner.hpp"
#include "../exceptions/LibraryLoadException.hpp"
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <map>

namespace arcade {
    class Core {
    protected:
        std::vector<std::string> displayLibraries;
        std::vector<std::string> gameLibraries;
        std::unique_ptr<IDisplayModule> displayModule;
        std::unique_ptr<IGameModule> gameModule;
        size_t currentDisplayIndex;
        size_t currentGameIndex;
        bool running;
        bool inMenu;
        std::string initialDisplayLib;

        virtual void scanLibraries();
        virtual bool loadDisplayModule(const std::string &libraryPath);
        virtual bool loadGameModule(const std::string &libraryPath);
        virtual void processEvents();
        virtual void update(float deltaTime);
        virtual void render();
        virtual void renderMenu();

        struct PlayerScore {
            std::string playerName;
            std::string gameName;
            int score;
        };
        std::string playerName;
        std::vector<PlayerScore> highScores;
        void loadScores();
        void saveScores();

    public:
        Core(const std::string &initialDisplayLib = "");
        virtual ~Core() = default;

        void init();
        void run();
        void stop();
        void nextDisplayModule();
        void nextGameModule();
        void restartGame();
        void returnToMenu();
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