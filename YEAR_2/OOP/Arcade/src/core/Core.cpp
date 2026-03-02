/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Mar, 2025                                                     *
 * Title           - B-OOP-400-TLS-4-1-arcade-nolan.dumoulin [WSL: Debian]              *
 * Description     -                                                                    *
 *     Core                                                                             *
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

#include "Core.hpp"


namespace arcade {
    Core::Core(const std::string &initialDisplayLib) : currentDisplayIndex(0), currentGameIndex(0), running(false), inMenu(true), initialDisplayLib(initialDisplayLib) {}

    void Core::init()
    {
        scanLibraries();

        if (displayLibraries.empty() || gameLibraries.empty()) {
            throw LibraryLoadException("No display or game libraries found");
        }

        if (!initialDisplayLib.empty()) {
            bool found = false;
            for (size_t i = 0; i < displayLibraries.size(); ++i) {
                std::filesystem::path displayPath(displayLibraries[i]);
                std::filesystem::path initialPath(initialDisplayLib);


                if (displayPath.filename() == initialPath.filename()) {
                    currentDisplayIndex = i;
                    found = true;
                    break;
                }
            }

            if (!found) {
                throw LibraryLoadException("Specified display library not found in lib directory");
            }
        }

        if (!loadDisplayModule(displayLibraries[currentDisplayIndex])) {
            throw LibraryLoadException("Failed to load display module: " + displayLibraries[currentDisplayIndex]);
        }

        if (!displayModule) {
            throw LibraryLoadException("Display module is null after loading");
        }

        loadScores();

        inMenu = true;
        running = true;
    }

    void Core::run()
    {
        auto lastTime = std::chrono::high_resolution_clock::now();

        if (!displayModule) {
            std::cerr << "Error: Display module is null before entering main loop" << std::endl;
            return;
        }

        try {
            bool isModuleOpen = displayModule->isOpen();
            std::cerr << "Display module isOpen() check before loop: " << (isModuleOpen ? "true" : "false") << std::endl;

            if (!isModuleOpen) {
                std::cerr << "Warning: Display module reports as not open, initializing again..." << std::endl;
                displayModule->init();
                isModuleOpen = displayModule->isOpen();
                std::cerr << "Display module isOpen() after re-init: " << (isModuleOpen ? "true" : "false") << std::endl;
            }

            while (running && displayModule && displayModule->isOpen()) {
                auto currentTime = std::chrono::high_resolution_clock::now();
                float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
                lastTime = currentTime;

                processEvents();
                update(deltaTime);
                render();

                int frameDelay = 16;
                std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay));
            }

            if (displayModule) {
                std::cerr << ", isOpen=" << (displayModule->isOpen() ? "true" : "false");
            }

            std::cerr << std::endl;
        } 
        catch (const std::exception& e) {
            std::cerr << "Exception in main loop: " << e.what() << std::endl;
        }
        catch (...) {
            std::cerr << "Unknown exception in main loop" << std::endl;
        }
    }

    void Core::stop()
    {
        saveScores();
        if (gameModule) {
            gameModule->stop();
            gameModule.reset();
        }

        if (displayModule) {
            displayModule->stop();
            displayModule.reset();
        }

        running = false;
    }

    void Core::scanLibraries()
    {
        displayLibraries.clear();
        gameLibraries.clear();

        try {
            std::vector<LibraryInfo> libraries = LibraryScanner::scanLibraries();

            for (const auto &library : libraries) {
                if (library.type == LibraryInfo::Type::Display) {
                    displayLibraries.push_back(library.path);
                } else if (library.type == LibraryInfo::Type::Game) {
                    gameLibraries.push_back(library.path);
                }
            }
        } catch (const LibraryLoadException &e) {
            std::cerr << "Warning during library scanning: " << e.what() << std::endl;
        }
    }

    bool Core::loadDisplayModule(const std::string &libraryPath)
    {
        try {
            if (displayModule) {
                displayModule->stop();
                displayModule.reset();
            }

            if (!std::filesystem::exists(libraryPath)) {
                throw LibraryLoadException("Display library file not found: " + libraryPath);
            }

            DynamicLoader<IDisplayModule> loader(libraryPath);
            auto newModule = loader.getInstance("createDisplayModule");

            if (!newModule) {
                throw LibraryLoadException("Failed to create display module instance");
            }

            displayModule.reset(newModule);
            
            displayModule->init();

            return true;
        } catch (const LibraryLoadException &e) {
            std::cerr << "Error loading display module: " << e.what() << std::endl;
            return false;
        }
    }

    bool Core::loadGameModule(const std::string &libraryPath)
    {
        try {
            if (gameModule) {
                gameModule->stop();
                gameModule.reset();
            }

            if (!std::filesystem::exists(libraryPath)) {
                throw LibraryLoadException("Game library file not found: " + libraryPath);
            }

            DynamicLoader<IGameModule> loader(libraryPath);
            auto newModule = loader.getInstance("createGameModule");

            if (!newModule) {
                throw LibraryLoadException("Failed to create game module instance");
            }

            gameModule.reset(newModule);

            gameModule->init();

            return true;
        } catch (const LibraryLoadException &e) {
            std::cerr << "Error loading game module: " << e.what() << std::endl;
            return false;
        }
    }

    void Core::processEvents()
    {
        InputEvent event;

        while (displayModule && displayModule->pollEvent(event)) {
            bool isNcurses = (displayModule->getName() == "NCurses");
            
            if (event.type == EventType::MouseButtonPressed && event.mouseButton.button == 0) {
                if (inMenu) {
                    int width = displayModule->getWidth();
                    int height = displayModule->getHeight();

                    int itemsStartY = isNcurses ? 4 : 140;
                    int itemSpacing = isNcurses ? 1 : 30;
                    int leftSectionMaxX = isNcurses ? width / 2 - 2 : width / 2;

                    if (event.mouseButton.x <= leftSectionMaxX) {
                        int relativeY = event.mouseButton.y - itemsStartY;
                        if (relativeY >= 0 && relativeY < static_cast<int>(displayLibraries.size() * itemSpacing)) {
                            int index = relativeY / itemSpacing;
                            if (index >= 0 && index < static_cast<int>(displayLibraries.size())) {
                                currentDisplayIndex = index;
                                loadDisplayModule(displayLibraries[currentDisplayIndex]);
                            }
                        }
                    }
                    else {
                        int relativeY = event.mouseButton.y - itemsStartY;
                        if (relativeY >= 0 && relativeY < static_cast<int>(gameLibraries.size() * itemSpacing)) {
                            int index = relativeY / itemSpacing;
                            if (index >= 0 && index < static_cast<int>(gameLibraries.size())) {
                                currentGameIndex = index;
                            }
                        }
                    }

                    if (!playerName.empty()) {
                        if (isNcurses) {
                            int startButtonY = height - 3;
                            if (event.mouseButton.y == startButtonY && 
                                event.mouseButton.x >= 2 && event.mouseButton.x <= 14) {
                                inMenu = false;
                                loadGameModule(gameLibraries[currentGameIndex]);
                            }
                        } else {
                            if (event.mouseButton.x >= width / 2 - 150 && 
                                event.mouseButton.x <= width / 2 + 150 &&
                                event.mouseButton.y >= height - 70 && 
                                event.mouseButton.y <= height - 45) {
                                inMenu = false;
                                loadGameModule(gameLibraries[currentGameIndex]);
                            }
                        }
                    }
                }
                else if (gameModule) {
                    gameModule->handleEvent(event);
                }
            } else if (event.type == EventType::KeyPressed) {
                if (inMenu) {
                    if (event.key.keyCode >= KeyCode::A && event.key.keyCode <= KeyCode::Z) {
                        char c = 'a' + (static_cast<int>(event.key.keyCode) - static_cast<int>(KeyCode::A));
                        if (playerName.length() < 20) {
                            playerName += c;
                        }
                    } 
                    else if (event.key.keyCode >= KeyCode::Num0 && event.key.keyCode <= KeyCode::Num9) {
                        char c = '0' + (static_cast<int>(event.key.keyCode) - static_cast<int>(KeyCode::Num0));
                        if (playerName.length() < 20) {
                            playerName += c;
                        }
                    }
                    else if (event.key.keyCode == KeyCode::Space) {
                        if (playerName.length() < 20 && !playerName.empty()) {
                            playerName += ' ';
                        }
                    }
                    else if (event.key.keyCode == KeyCode::Enter) {
                        if (!playerName.empty() && !gameLibraries.empty()) {
                            inMenu = false;
                            loadGameModule(gameLibraries[currentGameIndex]);
                        }
                    }
                }

                switch (event.key.keyCode) {
                    case KeyCode::Escape:
                        if (!inMenu) {
                            returnToMenu();
                        } else {
                            running = false;
                        }
                        break;

                    case KeyCode::Tab:
                        nextDisplayModule();
                        break;

                    case KeyCode::Shift:
                        nextGameModule();
                        break;

                    case KeyCode::R:
                        restartGame();
                        break;

                    case KeyCode::Backspace:
                        if (inMenu && !playerName.empty()) {
                            playerName.pop_back();
                        }
                        break;
                    default:
                        if (!inMenu && gameModule) {
                            gameModule->handleEvent(event);
                        }
                        break;
                }
            }
            
            else if (event.type == EventType::WindowClosed) {
                running = false;
            }
            else if (!inMenu && gameModule) {
                gameModule->handleEvent(event);
            }
        }
    }

    void Core::update(float deltaTime)
    {
        if (!inMenu && gameModule) {
            gameModule->update(deltaTime);

            if (gameModule->isGameOver()) {
                std::string gameName = LibraryScanner::getLibraryName(gameLibraries[currentGameIndex]);
                int score = gameModule->getScore();
                
                if (score > 0 && !playerName.empty()) {
                    bool shouldUpdate = true;
                    bool scoreExists = false;
                    
                    for (auto& existingScore : highScores) {
                        if (existingScore.playerName == playerName && existingScore.gameName == gameName) {
                            scoreExists = true;
                            if (score > existingScore.score) {
                                existingScore.score = score;
                                saveScores();
                            } else {
                                shouldUpdate = false;
                            }
                            break;
                        }
                    }

                    if (!scoreExists && shouldUpdate) {
                        PlayerScore newScore;
                        newScore.playerName = playerName;
                        newScore.gameName = gameName;
                        newScore.score = score;
                        
                        highScores.push_back(newScore);
                        saveScores();
                    }
                }
            }
        }
    }

    void Core::render()
    {
        if (displayModule) {
            displayModule->clear();
            if (inMenu) {
                renderMenu();
            } else if (gameModule) {
                gameModule->render(*displayModule);
            }
            displayModule->display();
        }
    }

    void Core::renderMenu()
    {
        if (!displayModule) return;

        int width = displayModule->getWidth();
        int height = displayModule->getHeight();

        bool isNcurses = (displayModule->getName() == "NCurses");

        if (!isNcurses) {
            displayModule->clear();
        }

        int titleX = isNcurses ? width / 2 - 3 : width / 2 - 50;
        int titleY = isNcurses ? 1 : 50;
        displayModule->drawText(titleX, titleY, "ARCADE", Colors::Red);

        int leftHeaderX = isNcurses ? 2 : 50;
        int rightHeaderX = isNcurses ? width / 2 + 5 : width - 250;
        int headerY = isNcurses ? 3 : 100;

        displayModule->drawText(leftHeaderX, headerY, "Display Libraries:", Colors::Blue);
        displayModule->drawText(rightHeaderX, headerY, "Game Libraries:", Colors::Blue);

        int leftItemsX = isNcurses ? 4 : 70;
        int rightItemsX = isNcurses ? width / 2 + 7 : width - 230;
        int itemsStartY = isNcurses ? 4 : 140;
        int itemSpacing = isNcurses ? 1 : 30;

        if (isNcurses) {
            int maxDisplayWidth = 0;
            for (const auto& lib : displayLibraries) {
                maxDisplayWidth = std::max(maxDisplayWidth, static_cast<int>(LibraryScanner::getLibraryName(lib).length()));
            }
            
            for (size_t i = 0; i < displayLibraries.size(); ++i) {
                displayModule->drawText(leftItemsX - 2, itemsStartY + static_cast<int>(i) * itemSpacing, "[", Colors::Cyan);
                displayModule->drawText(leftItemsX + maxDisplayWidth + 1, itemsStartY + static_cast<int>(i) * itemSpacing, "]", Colors::Cyan);
            }
            
            int maxGameWidth = 0;
            for (const auto& lib : gameLibraries) {
                maxGameWidth = std::max(maxGameWidth, static_cast<int>(LibraryScanner::getLibraryName(lib).length()));
            }
            
            for (size_t i = 0; i < gameLibraries.size(); ++i) {
                displayModule->drawText(rightItemsX - 2, itemsStartY + static_cast<int>(i) * itemSpacing, "[", Colors::Cyan);
                displayModule->drawText(rightItemsX + maxGameWidth + 1, itemsStartY + static_cast<int>(i) * itemSpacing, "]", Colors::Cyan);
            }
        }

        for (size_t i = 0; i < displayLibraries.size(); ++i) {
            std::string name = LibraryScanner::getLibraryName(displayLibraries[i]);
            Color color = (i == currentDisplayIndex) ? Colors::Green : Colors::White;
            displayModule->drawText(leftItemsX, itemsStartY + i * itemSpacing, name, color);
        }
        
        for (size_t i = 0; i < gameLibraries.size(); ++i) {
            std::string name = LibraryScanner::getLibraryName(gameLibraries[i]);
            Color color = (i == currentGameIndex) ? Colors::Green : Colors::White;
            displayModule->drawText(rightItemsX, itemsStartY + i * itemSpacing, name, color);
        }
        
        int scoresHeaderX = isNcurses ? width / 2 - 5 : width / 2 - 110;
        int scoresHeaderY = isNcurses ? 6 + displayLibraries.size() : 170;
        displayModule->drawText(scoresHeaderX, scoresHeaderY, "High Scores:", Colors::Yellow);

        if (!highScores.empty()) {
            std::map<std::string, std::vector<PlayerScore>> scoresByGame;
            for (const auto& score : highScores) {
                scoresByGame[score.gameName].push_back(score);
            }
            
            for (auto& [game, scores] : scoresByGame) {
                std::sort(scores.begin(), scores.end(), 
                    [](const PlayerScore &a, const PlayerScore &b) {
                        return a.score > b.score;
                    });
            }
            
            int scoresX = isNcurses ? width / 2 - 15 : width / 2 - 150;
            int scoresStartY = isNcurses ? 7 + displayLibraries.size() : 195;
            int scoreSpacing = isNcurses ? 1 : 25;
            int currentLine = 0;
            
            auto minesweeperIt = scoresByGame.find("minesweeper");
            if (minesweeperIt != scoresByGame.end()) {
                int count = std::min(3, static_cast<int>(minesweeperIt->second.size()));
                for (int i = 0; i < count; ++i) {
                    const auto& score = minesweeperIt->second[i];
                    std::string scoreText = score.playerName + " - " + score.gameName + " - " + std::to_string(score.score);
                    displayModule->drawText(scoresX, scoresStartY + currentLine * scoreSpacing, scoreText, Colors::White);
                    currentLine++;
                }
            }
            
            auto snakeIt = scoresByGame.find("snake");
            if (snakeIt != scoresByGame.end()) {
                int count = std::min(3, static_cast<int>(snakeIt->second.size()));
                for (int i = 0; i < count; ++i) {
                    const auto& score = snakeIt->second[i];
                    std::string scoreText = score.playerName + " - " + score.gameName + " - " + std::to_string(score.score);
                    displayModule->drawText(scoresX, scoresStartY + currentLine * scoreSpacing, scoreText, Colors::White);
                    currentLine++;
                }
            }
        }

        int nameInputBaseY = std::min(6, static_cast<int>(highScores.size()));
        int nameInputY = isNcurses ? 13 + displayLibraries.size() + nameInputBaseY : 230 + nameInputBaseY * 25;
        int namePromptX = isNcurses ? width / 2 - 5 : width / 2 - 100;

        displayModule->drawText(namePromptX, nameInputY, "Your Name:", Colors::Yellow);

        int nameInputFieldY = isNcurses ? nameInputY + 1 : nameInputY + 30;
        int nameInputFieldX = isNcurses ? namePromptX : width / 2 - 95;

        if (!isNcurses) {
            int nameWidth = playerName.length() * 10 + 20;
            displayModule->drawRect(width / 2 - 100, nameInputY + 25, nameWidth, 25, Color(30, 30, 30));
        }

        displayModule->drawText(nameInputFieldX, nameInputFieldY, playerName + "_", Colors::White);

        int instructionsX = isNcurses ? 2 : width / 2 - 150;
        int instructionsStartY = isNcurses ? height - 5 : height - 120;
        int instructionSpacing = isNcurses ? 1 : 25;

        displayModule->drawText(instructionsX, instructionsStartY, "Press Tab to change graphics library", Colors::Yellow);
        displayModule->drawText(instructionsX, instructionsStartY + instructionSpacing, "Press Shift to change game library", Colors::Yellow);
        displayModule->drawText(instructionsX, instructionsStartY + 2 * instructionSpacing, "Press Enter to start game", Colors::Yellow);
        displayModule->drawText(instructionsX, instructionsStartY + 3 * instructionSpacing, "Press Escape to quit", Colors::Yellow);
    }

    void Core::nextDisplayModule()
    {
        if (displayLibraries.empty()) {
            return;
        }

        currentDisplayIndex = (currentDisplayIndex + 1) % displayLibraries.size();
        loadDisplayModule(displayLibraries[currentDisplayIndex]);
    }

    void Core::nextGameModule()
    {
        if (gameLibraries.empty()) {
            return;
        }

        currentGameIndex = (currentGameIndex + 1) % gameLibraries.size();

        if (!inMenu) {
            loadGameModule(gameLibraries[currentGameIndex]);
        }
    }

    void Core::restartGame()
    {
        if (!inMenu && gameModule) {
            gameModule->reset();
        }
    }

    void Core::returnToMenu()
    {
        if (gameModule) {
            gameModule->stop();
            gameModule.reset();
        }

        inMenu = true;
    }

    void Core::loadScores()
    {
        highScores.clear();
        std::ifstream file("scores.txt");
        if (!file.is_open()) {
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            PlayerScore score;

            if (!(iss >> score.playerName)) {
                continue;
            }

            std::replace(score.playerName.begin(), score.playerName.end(), '_', ' ');

            if (!(iss >> score.gameName)) {
                continue;
            }

            if (!(iss >> score.score)) {
                continue;
            }

            if (score.score > 0) {
                bool isDuplicate = false;
                for (auto& existingScore : highScores) {
                    if (existingScore.playerName == score.playerName && 
                        existingScore.gameName == score.gameName) {
                        if (score.score > existingScore.score) {
                            existingScore.score = score.score;
                        }
                        isDuplicate = true;
                        break;
                    }
                }

                if (!isDuplicate) {
                    highScores.push_back(score);
                }
            }
        }
        file.close();
    }
    void Core::saveScores()
    {
        std::ofstream file("scores.txt");
        if (!file.is_open()) {
            std::cerr << "Failed to save scores" << std::endl;
            return;
        }

        std::map<std::string, std::vector<PlayerScore>> scoresByGame;
        for (const auto& score : highScores) {
            scoresByGame[score.gameName].push_back(score);
        }
        
        for (auto& [game, scores] : scoresByGame) {
            std::sort(scores.begin(), scores.end(), 
                [](const PlayerScore &a, const PlayerScore &b) {
                    return a.score > b.score;
                });
        }
        
        for (const auto& [game, scores] : scoresByGame) {
            int count = std::min(3, static_cast<int>(scores.size()));
            for (int i = 0; i < count; i++) {
                const auto& score = scores[i];
                std::string safePlayerName = score.playerName;
                std::replace(safePlayerName.begin(), safePlayerName.end(), ' ', '_');
                file << safePlayerName << " " << score.gameName << " " << score.score << std::endl;
            }
        }
        file.close();
        loadScores();
    }
}