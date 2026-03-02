/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sun, Feb, 2025                                                     *
 * Title           - B-PDG-300-TLS-3-1-PDGRUSH3-sven.reichert [WSL: Debian]             *
 * Description     -                                                                    *
 *     main                                                                             *
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

#include <iostream>
#include <memory>
#include "../include/MonitorCore.hpp"
#include "../include/HostnameModule.hpp"
#include "../include/OsModule.hpp"
#include "../include/DateTimeModule.hpp"
#include "../include/CpuModule.hpp"
#include "../include/RamModule.hpp"
#include "../include/NcursesDisplay.hpp"
#include "../include/SfmlDisplay.hpp"

int main(int ac, char **av)
{
        Krell::MonitorCore core;

        core.addModule(std::make_shared<Krell::HostnameModule>());
        core.addModule(std::make_shared<Krell::OSModule>());
        core.addModule(std::make_shared<Krell::DateTimeModule>());
        core.addModule(std::make_shared<Krell::CPUModule>());
        core.addModule(std::make_shared<Krell::RAMModule>());

        if (ac == 2 && av[1][0] == '-') {
            std::string arg = av[1];
            switch (arg[1]) {
            case 'g':
                if (arg == "-g") {
                    core.setDisplay(std::make_shared<Krell::SFMLDisplay>());
                    //std::cout << "SFML display not implemented yet." << std::endl;
                    break;
                }
            case 't':
                if (arg == "-t") {
                    core.setDisplay(std::make_shared<Krell::NcursesDisplay>());
                    //std::cout << "Ncurses display set." << std::endl;
                    break;
                }
            default:
                std::cerr << "Usage: " << av[0] << " [-g | -t]" << std::endl;
                return 84;
            }
        } else {
            std::cerr << "Usage: " << av[0] << " [-g | -t]" << std::endl;
            return 84;
        }
        core.run();
    return 0;
}