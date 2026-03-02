/*
** EPITECH PROJECT, 2025
** B-OOP-400-TLS-4-1-arcade-nolan.dumoulin
** File description:
** Minesweeper
*/

#include "Minesweeper.hpp"

namespace arcade {

    Minesweeper::Minesweeper() : AGameModule("Minesweeper"), gridWidth(BEGINNER_WIDTH), gridHeight(BEGINNER_HEIGHT), mineCount(BEGINNER_MINES), revealedCount(0), flagCount(0), firstClick(true), won(false), timer(0)
    {
        srand(static_cast<unsigned int>(time(nullptr)));
        initGrid();
    }

    Minesweeper::~Minesweeper()
    {
        stop();
    }

    void Minesweeper::init()
    {
        reset();
    }

    void Minesweeper::stop()
    {
    }

    void Minesweeper::reset()
    {
        revealedCount = 0;
        flagCount = 0;
        firstClick = true;
        won = false;
        timer = 0;
        score = 0;
        gameOver = false;
        initGrid();
    }

    void Minesweeper::initGrid()
    {
        grid.resize(gridHeight);
        for (int y = 0; y < gridHeight; ++y) {
            grid[y].resize(gridWidth);
            for (int x = 0; x < gridWidth; ++x) {
                grid[y][x] = Cell();
            }
        }
    }

    void Minesweeper::placeMines(int firstClickX, int firstClickY)
    {
        int minesPlaced = 0;
        while (minesPlaced < mineCount) {
            int x = rand() % gridWidth;
            int y = rand() % gridHeight;
            if (abs(x - firstClickX) <= 1 && abs(y - firstClickY) <= 1) {
                continue;
            }
            if (!grid[y][x].isMine) {
                grid[y][x].isMine = true;
                minesPlaced++;
            }
        }
        calculateAdjacentMines();
    }

    void Minesweeper::calculateAdjacentMines()
    {
        for (int y = 0; y < gridHeight; ++y) {
            for (int x = 0; x < gridWidth; ++x) {
                if (grid[y][x].isMine) {
                    continue;
                }
                int count = 0;
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx < 0 || nx >= gridWidth || ny < 0 || ny >= gridHeight) {
                            continue;
                        }
                        if (grid[ny][nx].isMine) {
                            count++;
                        }
                    }
                }
                grid[y][x].adjacentMines = count;
            }
        }
    }

    void Minesweeper::revealCell(int x, int y)
    {
        if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight) {
            return;
        }
        if (grid[y][x].state != CellState::Hidden) {
            return;
        }
        if (firstClick) {
            firstClick = false;
            placeMines(x, y);
        }
        grid[y][x].state = CellState::Revealed;
        revealedCount++;
        score += 10;
        if (grid[y][x].isMine) {
            gameOver = true;
            for (int cy = 0; cy < gridHeight; ++cy) {
                for (int cx = 0; cx < gridWidth; ++cx) {
                    if (grid[cy][cx].isMine && grid[cy][cx].state == CellState::Hidden) {
                        grid[cy][cx].state = CellState::Revealed;
                    }
                }
            }
            return;
        }
        if (grid[y][x].adjacentMines == 0) {
            revealEmptyCells(x, y);
        }
        if (checkWin()) {
            won = true;
            gameOver = true;
            score += 500 + static_cast<int>(1000 / timer);
        }
    }

    void Minesweeper::revealEmptyCells(int x, int y)
    {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                int nx = x + dx;
                int ny = y + dy;
                if (nx < 0 || nx >= gridWidth || ny < 0 || ny >= gridHeight) {
                    continue;
                }
                if (grid[ny][nx].state == CellState::Revealed) {
                    continue;
                }
                grid[ny][nx].state = CellState::Revealed;
                revealedCount++;
                score += 5;
                if (grid[ny][nx].adjacentMines == 0) {
                    revealEmptyCells(nx, ny);
                }
            }
        }
    }

    void Minesweeper::flagCell(int x, int y)
    {
        if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight) {
            return;
        }
        if (grid[y][x].state == CellState::Revealed) {
            return;
        }
        if (grid[y][x].state == CellState::Hidden) {
            grid[y][x].state = CellState::Flagged;
            flagCount++;
        } else if (grid[y][x].state == CellState::Flagged) {
            grid[y][x].state = CellState::Hidden;
            flagCount--;
        }
    }

    bool Minesweeper::checkWin()
    {
        return revealedCount == (gridWidth * gridHeight - mineCount);
    }

    int Minesweeper::getCellSize(const IDisplayModule &display) const
    {
        if (display.getName() == "NCurses") {
            return 1;
        }
        
        int maxCellWidth = display.getWidth() / gridWidth;
        int maxCellHeight = display.getHeight() / gridHeight;
        return std::min(maxCellWidth, maxCellHeight);
    }

    bool Minesweeper::getGridCoords(const IDisplayModule &display, int displayX, int displayY, int &gridX, int &gridY) const
    {
        bool isNcurses = (display.getName() == "NCurses");
        int cellSize = isNcurses ? 1 : getCellSize(display);
        int offsetX, offsetY;
        
        if (isNcurses) {
            offsetX = display.getWidth() / 2 - gridWidth;
            offsetY = display.getHeight() / 2 - gridHeight / 2;
            
            displayX -= offsetX;
            displayY -= offsetY;
            
            gridX = displayX / 2;
            gridY = displayY;
        } else {
            offsetX = (display.getWidth() - (gridWidth * cellSize)) / 2;
            offsetY = (display.getHeight() - (gridHeight * cellSize)) / 2;
            
            displayX -= offsetX;
            displayY -= offsetY;
            gridX = displayX / cellSize;
            gridY = displayY / cellSize;
        }
        
        return (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight);
    }

    void Minesweeper::update(float deltaTime)
    {
        if (!gameOver && !firstClick) {
            timer += deltaTime;
        }
    }

    void Minesweeper::render(IDisplayModule &displayModule)
    {
        this->displayModule = &displayModule;
        bool isNcurses = (displayModule.getName() == "NCurses");
        
        int cellSize = isNcurses ? 1 : getCellSize(displayModule);
        
        int offsetX, offsetY;
        if (isNcurses) {
            offsetX = displayModule.getWidth() / 2 - gridWidth;
            offsetY = displayModule.getHeight() / 2 - gridHeight / 2;
        } else {
            offsetX = (displayModule.getWidth() - (gridWidth * cellSize)) / 2;
            offsetY = (displayModule.getHeight() - (gridHeight * cellSize)) / 2;
        }
        
        if (isNcurses) {
            for (int y = 0; y <= gridHeight; y++) {
                for (int x = 0; x <= gridWidth; x++) {
                    displayModule.drawText(offsetX + x * 2, offsetY + y, "+", Colors::Cyan);
                }
            }
            
            for (int y = 0; y <= gridHeight; y++) {
                for (int x = 0; x < gridWidth; x++) {
                    displayModule.drawText(offsetX + x * 2 + 1, offsetY + y, "-", Colors::Cyan);
                }
            }
            
            for (int y = 0; y < gridHeight; y++) {
                for (int x = 0; x <= gridWidth; x++) {
                    displayModule.drawText(offsetX + x * 2, offsetY + y + 0.5, "|", Colors::Cyan);
                }
            }
            
            for (int y = 0; y < gridHeight; y++) {
                for (int x = 0; x < gridWidth; x++) {
                    int drawX = offsetX + x * 2 + 1;
                    int drawY = offsetY + y;
                    
                    char cellChar = '.';
                    Color cellColor = Colors::White;
                    
                    switch (grid[y][x].state) {
                        case CellState::Hidden:
                            cellChar = '.';
                            cellColor = Colors::White;
                            break;
                        case CellState::Flagged:
                            cellChar = 'F';
                            cellColor = Colors::Red;
                            break;
                        case CellState::Revealed:
                            if (grid[y][x].isMine) {
                                cellChar = '*';
                                cellColor = Colors::Red;
                            } else if (grid[y][x].adjacentMines > 0) {
                                cellChar = '0' + grid[y][x].adjacentMines;
                                
                                switch (grid[y][x].adjacentMines) {
                                    case 1: cellColor = Colors::Blue; break;
                                    case 2: cellColor = Colors::Green; break;
                                    case 3: cellColor = Colors::Red; break;
                                    case 4: cellColor = Colors::Magenta; break;
                                    case 5: cellColor = Colors::Yellow; break;
                                    case 6: cellColor = Colors::Cyan; break;
                                    case 7: cellColor = Colors::White; break;
                                    case 8: cellColor = Colors::White; break;
                                    default: cellColor = Colors::White; break;
                                }
                            } else {
                                cellChar = ' ';
                                cellColor = Colors::White;
                            }
                            break;
                    }
                    
                    displayModule.drawText(drawX, drawY, std::string(1, cellChar), cellColor);
                }
            }
        } else {
            for (int y = 0; y < gridHeight; ++y) {
                for (int x = 0; x < gridWidth; ++x) {
                    int drawX = offsetX + x * cellSize;
                    int drawY = offsetY + y * cellSize;
                    
                    Color bgColor;
                    switch (grid[y][x].state) {
                        case CellState::Hidden:
                            bgColor = Color(150, 150, 150);
                            break;
                        case CellState::Flagged:
                            bgColor = Color(255, 165, 0);
                            break;
                        case CellState::Revealed:
                            if (grid[y][x].isMine) {
                                bgColor = Color(255, 0, 0);
                            } else {
                                bgColor = Color(220, 220, 220);
                            }
                            break;
                    }
                    
                    displayModule.drawRect(drawX, drawY, cellSize, cellSize, bgColor);
                    
                    if (grid[y][x].state == CellState::Revealed) {
                        if (grid[y][x].isMine) {
                            displayModule.drawCircle(drawX + cellSize / 2, drawY + cellSize / 2, cellSize / 4, Colors::Black);
                        } else if (grid[y][x].adjacentMines > 0) {
                            std::string text = std::to_string(grid[y][x].adjacentMines);
                            Color textColor;
                            switch (grid[y][x].adjacentMines) {
                                case 1: textColor = Color(0, 0, 255); break;      // Blue
                                case 2: textColor = Color(0, 128, 0); break;      // Green
                                case 3: textColor = Color(255, 0, 0); break;      // Red
                                case 4: textColor = Color(128, 0, 128); break;    // Purple
                                case 5: textColor = Color(128, 0, 0); break;      // Maroon
                                case 6: textColor = Color(0, 128, 128); break;    // Teal
                                case 7: textColor = Color(0, 0, 0); break;        // Black
                                case 8: textColor = Color(128, 128, 128); break;  // Gray
                                default: textColor = Colors::Black; break;
                            }
                            displayModule.drawText(drawX + cellSize / 3, drawY + cellSize / 4, text, textColor);
                        }
                    } else if (grid[y][x].state == CellState::Flagged) {
                        displayModule.drawRect(drawX + cellSize / 3, drawY + cellSize / 4, cellSize / 10, cellSize / 2, Colors::Black);
                        displayModule.drawRect(drawX + cellSize / 3, drawY + cellSize / 4, cellSize / 3, cellSize / 4, Colors::Red);
                    }
                    
                    displayModule.drawRect(drawX, drawY, cellSize, 1, Colors::Black);
                    displayModule.drawRect(drawX, drawY, 1, cellSize, Colors::Black);
                    displayModule.drawRect(drawX + cellSize - 1, drawY, 1, cellSize, Colors::Black);
                    displayModule.drawRect(drawX, drawY + cellSize - 1, cellSize, 1, Colors::Black);
                }
            }
        }
        
        std::string timeText = "Time: " + std::to_string(static_cast<int>(timer));
        std::string minesText = "Mines: " + std::to_string(mineCount - flagCount);
        std::string scoreText = "Score: " + std::to_string(score);
        
        if (isNcurses) {
            displayModule.drawText(10, 10, timeText, Colors::White);
            displayModule.drawText(10, 12, minesText, Colors::White);
            displayModule.drawText(10, 14, scoreText, Colors::White);
        } else {
            displayModule.drawText(10, 20, timeText, Colors::White);
            displayModule.drawText(10, 50, minesText, Colors::White);
            displayModule.drawText(10, 80, scoreText, Colors::White);
        }
        
        if (gameOver) {
            std::string statusText = won ? "You Win!" : "Game Over!";
            int statusX = isNcurses ? displayModule.getWidth() / 2 - statusText.length() / 2 : displayModule.getWidth() / 2 - 50;
            int statusY = isNcurses ? offsetY - 2 : 20;
            displayModule.drawText(statusX, statusY, statusText, won ? Colors::Green : Colors::Red);
            
            std::string restartText = "Press R to restart";
            int restartX = isNcurses ? displayModule.getWidth() / 2 - restartText.length() / 2 : displayModule.getWidth() / 2 - 100;
            int restartY = isNcurses ? offsetY - 1 : 40;
            displayModule.drawText(restartX, restartY, restartText, Colors::White);
        }
        if (isNcurses) {
            std::string prompt = "Left click to reveal a cell. Right click to place a flag.";
            int promptX = displayModule.getWidth() / 2 - prompt.length() / 2;
            int promptY = offsetY + gridHeight + 2;
            displayModule.drawText(promptX, promptY, prompt, Colors::White);
        }
    }

    void Minesweeper::handleEvent(const InputEvent &event)
    {
        if (gameOver) {
            if (event.type == EventType::KeyPressed && event.key.keyCode == KeyCode::R) {
                reset();
            }
            return;
        }
        
        if (event.type == EventType::MouseButtonPressed) {
            int gridX, gridY;
            if (getGridCoords(*displayModule, event.mouseButton.x, event.mouseButton.y, gridX, gridY)) {
                if (event.mouseButton.button == 0) {
                    revealCell(gridX, gridY);
                } else if (event.mouseButton.button == 2) {
                    flagCell(gridX, gridY);
                }
            }
        } else if (event.type == EventType::KeyPressed && event.key.keyCode == KeyCode::R) {
            reset();
        }
    }

    extern "C" {
        IGameModule *createGameModule() {
            return new Minesweeper();
        }
    }
}
