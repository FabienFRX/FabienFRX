/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sun, Feb, 2025                                                     *
 * Title           - B-PDG-300-TLS-3-1-PDGRUSH3-sven.reichert [WSL: Debian]             *
 * Description     -                                                                    *
 *     MonitorCore                                                                      *
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

#include "../include/MonitorCore.hpp"
#include "../include/SfmlDisplay.hpp"
#include "../include/NcursesDisplay.hpp"

void Krell::MonitorCore::addModule(std::shared_ptr<IModule> module)
{
    modules.push_back(module);
}

void Krell::MonitorCore::setDisplay(std::shared_ptr<IDisplay> display)
{
    this->display = display;
}

void Krell::MonitorCore::run()
{
    if (display == nullptr) {
        throw std::runtime_error("No display set.");
    }
    if (modules.empty()) {
        throw std::runtime_error("No modules added.");
    }

    auto sfmlDisplay = std::dynamic_pointer_cast<Krell::SFMLDisplay>(display);
    auto ncursesDisplay = std::dynamic_pointer_cast<Krell::NcursesDisplay>(display);

    if (sfmlDisplay) {
        while (sfmlDisplay->getWindow().isOpen()) {
            sf::Event event;
            while (sfmlDisplay->getWindow().pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    sfmlDisplay->getWindow().close();
                }
            }

            std::string data;
            for (auto &module : modules) {
                module->update();
                data += module->getData() + "\n";
            }
            sfmlDisplay->draw(data);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } else if (ncursesDisplay) {
        while (true) {
            std::string data;
            for (auto &module : modules) {
                module->update();
                data += module->getData() + "\n";
            }
            ncursesDisplay->draw(data);
            std::this_thread::sleep_for(std::chrono::seconds(1));

            // press q to quit the loop in ncurses
            int ch = getch();
            if (ch == 'q' || ch == 'Q') {
                break;
            }
        }
    } else {
        throw std::runtime_error("Unknown display type.");
    }
}