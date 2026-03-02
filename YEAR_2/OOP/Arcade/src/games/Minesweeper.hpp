/*
** EPITECH PROJECT, 2025
** B-OOP-400-TLS-4-1-arcade-nolan.dumoulin
** File description:
** Minesweeper
*/

#pragma once

#include "../interfaces/AGameModule.hpp"
#include "../interfaces/IDisplayModule.hpp"
#include <algorithm>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

namespace arcade {
    enum class CellState {
        Hidden,
        Revealed,
        Flagged
    };

    struct Cell {
        bool isMine;
        CellState state;
        int adjacentMines;
        Cell() : isMine(false), state(CellState::Hidden), adjacentMines(0) {}
    };

    class Minesweeper : public AGameModule {
    private:
        static constexpr int BEGINNER_WIDTH = 9;
        static constexpr int BEGINNER_HEIGHT = 9;
        static constexpr int BEGINNER_MINES = 10;

        IDisplayModule *displayModule;
        
        int gridWidth;
        int gridHeight;
        int mineCount;
        int revealedCount;
        int flagCount;
        bool firstClick;
        bool won;
        float timer;
        
        std::vector<std::vector<Cell>> grid;
        
        void initGrid();
        void placeMines(int firstClickX, int firstClickY);
        void calculateAdjacentMines();
        void revealCell(int x, int y);
        void revealEmptyCells(int x, int y);
        void flagCell(int x, int y);
        bool checkWin();
        int getCellSize(const IDisplayModule &display) const;
        
    public:
        Minesweeper();
        ~Minesweeper() override;
        void init() override;
        void stop() override;
        void update(float deltaTime) override;
        void render(IDisplayModule &displayModule) override;
        void handleEvent(const InputEvent &event) override;
        void reset() override;
        bool getGridCoords(const IDisplayModule &display, int displayX, int displayY, int &gridX, int &gridY) const;
    };
    extern "C" {
        IGameModule *createGameModule();
    }
}
