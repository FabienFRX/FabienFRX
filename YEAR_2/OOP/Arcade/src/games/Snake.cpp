/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sat, Mar, 2025                                                     *
 * Title           - B-OOP-400-TLS-4-1-arcade-nolan.dumoulin [WSL: Debian]              *
 * Description     -                                                                    *
 *     Snake                                                                            *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄             *
 *      ▐  ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀            *
 *        █▄▄▄▄▄  ▐  █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             *
 *        █    ▌     █          █        █        █    ▌    █         █   █             *
 *       ▄▀▄▄▄▄    ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             *
 *       █    ▐   █         █       █ █          █    ▐   █     ▐   █   █               *
 *       ▐        ▐         ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "Snake.hpp"

namespace arcade {
    Snake::Snake() : AGameModule("Snake"), m_nextDirection({0, 0}), m_directionChanged(false), m_width(40),  m_height(30), m_gameOver(false), m_score(0), moveTimer(0), moveDelay(0.2f)
    {
        std::srand(time(nullptr));
    }

    void Snake::stop() {}

    void Snake::init()
    {
        reset();
    }

    void Snake::reset()
    {
        m_snake.clear();

        int startX = m_width / 2;
        int startY = m_height / 2;

        for (int i = 0; i < 4; ++i) {
            m_snake.push_back({startX - i, startY});
        }

        m_direction = {1, 0};
        m_nextDirection = {0, 0};
        m_directionChanged = false;

        generateFood();
        m_score = 0;
        m_gameOver = false;

        moveTimer = 0;
    }

    void Snake::update(float deltaTime)
    {
        if (m_gameOver)
            return;

        moveTimer += deltaTime;
        if (moveTimer >= moveDelay) {
            moveSnake();
            moveTimer = 0;
            m_directionChanged = false;
        }
    }

    void Snake::moveSnake()
    {
        if (m_nextDirection.x != 0 || m_nextDirection.y != 0) {
            m_direction = m_nextDirection;
            m_nextDirection = {0, 0};
        }
        
        Position newHead = {m_snake.front().x + m_direction.x, m_snake.front().y + m_direction.y};

        if (checkCollision(newHead)) {
            m_gameOver = true;
            gameOver = true;
            score = m_score;
            return;
        }
        m_snake.insert(m_snake.begin(), newHead);
        if (newHead.x == m_food.x && newHead.y == m_food.y) {
            m_score += 10;
            score = m_score;
            generateFood();
        } else {
            m_snake.pop_back();
        }
    }

    void Snake::handleEvent(const InputEvent &event)
    {
        if (event.type != EventType::KeyPressed || m_directionChanged)
            return;

        Position newDirection = m_direction;

        switch (event.key.keyCode) {
            case KeyCode::Up:
                if (m_direction.y != 1) newDirection = {0, -1};
                break;
            case KeyCode::Down:
                if (m_direction.y != -1) newDirection = {0, 1};
                break;
            case KeyCode::Left:
                if (m_direction.x != 1) newDirection = {-1, 0};
                break;
            case KeyCode::Right:
                if (m_direction.x != -1) newDirection = {1, 0};
                break;
            default:
                return;
        }

        if ((newDirection.x != m_direction.x || newDirection.y != m_direction.y)) {
            m_nextDirection = newDirection;
            m_directionChanged = true;
        }
    }

    void Snake::render(IDisplayModule &displayModule)
    {
        int cellSize = calculateCellSize(displayModule);
        int offsetX = calculateOffsetX(displayModule, cellSize);
        int offsetY = calculateOffsetY(displayModule, cellSize);
        bool isNcurses = (displayModule.getName() == "NCurses");

        if (!isNcurses) {
            displayModule.drawRect(0, 0, displayModule.getWidth(), displayModule.getHeight(), Color(0, 0, 0));
        }

        if (isNcurses) {
            for (int x = 0; x < m_width; x++) {
                displayModule.drawText(offsetX + x, offsetY - 1, "-", Colors::White);
                displayModule.drawText(offsetX + x, offsetY + m_height, "-", Colors::White);
            }
            for (int y = 0; y < m_height; y++) {
                displayModule.drawText(offsetX - 1, offsetY + y, "|", Colors::White);
                displayModule.drawText(offsetX + m_width, offsetY + y, "|", Colors::White);
            }
        } else {
            displayModule.drawRect(offsetX - 1, offsetY - 1, m_width * cellSize + 2, 1, Colors::White);
            displayModule.drawRect(offsetX - 1, offsetY + m_height * cellSize, m_width * cellSize + 2, 1, Colors::White);
            displayModule.drawRect(offsetX - 1, offsetY - 1, 1, m_height * cellSize + 2, Colors::White);
            displayModule.drawRect(offsetX + m_width * cellSize, offsetY - 1, 1, m_height * cellSize + 2, Colors::White);
        }

        for (size_t i = 0; i < m_snake.size(); ++i) {
            if (isNcurses) {
                char snakeChar = (i == 0) ? 'O' : 'o';
                displayModule.drawText(offsetX + m_snake[i].x, offsetY + m_snake[i].y, 
                                      std::string(1, snakeChar), (i == 0) ? Colors::Green : Colors::White);
            } else {
                Color color = (i == 0) ? Colors::Green : Colors::White;
                displayModule.drawRect(offsetX + m_snake[i].x * cellSize, offsetY + m_snake[i].y * cellSize, 
                                      cellSize, cellSize, color);
            }
        }
        
        if (isNcurses) {
            displayModule.drawText(offsetX + m_food.x, offsetY + m_food.y, "*", Colors::Red);
        } else {
            displayModule.drawRect(offsetX + m_food.x * cellSize, offsetY + m_food.y * cellSize, 
                                 cellSize, cellSize, Colors::Red);
        }

        std::string scoreText = "Score: " + std::to_string(m_score);
        displayModule.drawText(10, 10, scoreText, Colors::White);

        if (m_gameOver) {
            std::string gameOverText = "Game Over";
            int gameOverX = (displayModule.getWidth() - gameOverText.length() * (isNcurses ? 1 : 8)) / 2;
            int gameOverY = displayModule.getHeight() / 2 - (isNcurses ? 1 : 10);
            
            displayModule.drawText(gameOverX, gameOverY, gameOverText, Colors::Red);
            
            std::string restartText = "Press R to restart";
            int restartX = (displayModule.getWidth() - restartText.length() * (isNcurses ? 1 : 8)) / 2;
            int restartY = gameOverY + (isNcurses ? 2 : 20);
            
            displayModule.drawText(restartX, restartY, restartText, Colors::White);
        }
    }

    void Snake::generateFood()
    {
        while (true) {
            Position newFood = {std::rand() % m_width, std::rand() % m_height};

            if (std::find(m_snake.begin(), m_snake.end(), newFood) == m_snake.end()) {
                m_food = newFood;
                break;
            }
        }
    }

    bool Snake::checkCollision(const Position &pos) const
    {
        if (pos.x < 0 || pos.x >= m_width || pos.y < 0 || pos.y >= m_height)
            return true;
            
        for (size_t i = 0; i < m_snake.size() - 1; i++) {
            if (m_snake[i] == pos)
                return true;
        }
        return false;
    }

    int Snake::calculateCellSize(const IDisplayModule &displayModule) const
    {
        if (displayModule.getName() == "NCurses") {
            return 1;
        }
        
        int maxCellWidth = displayModule.getWidth() / (m_width + 2);
        int maxCellHeight = displayModule.getHeight() / (m_height + 2);
        
        return std::max(1, std::min(maxCellWidth, maxCellHeight));
    }

    int Snake::calculateOffsetX(const IDisplayModule &displayModule, int cellSize) const
    {
        return (displayModule.getWidth() - m_width * cellSize) / 2;
    }

    int Snake::calculateOffsetY(const IDisplayModule &displayModule, int cellSize) const
    {
        return (displayModule.getHeight() - m_height * cellSize) / 2;
    }

    extern "C" {
        IGameModule *createGameModule() {
            return new Snake();
        }
    }
}