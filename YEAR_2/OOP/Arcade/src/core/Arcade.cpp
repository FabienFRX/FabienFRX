/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sat, Mar, 2025                                                     *
 * Title           - B-OOP-400-TLS-4-1-arcade-nolan.dumoulin [WSL: Debian]              *
 * Description     -                                                                    *
 *     Arcade                                                                           *
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
#include "../exceptions/ArcadeException.hpp"
#include "../utils/LibraryScanner.hpp"
#include <iostream>
#include <memory>
#include <filesystem>

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "Error: Invalid number of arguments" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <graphics_library.so>" << std::endl;
        return 84;
    }
    
    if (!std::filesystem::exists(argv[1])) {
        std::cerr << "Error: Library file '" << argv[1] << "' not found" << std::endl;
        return 84;
    }

    std::string libName = arcade::LibraryScanner::getLibraryName(argv[1]);
    if (arcade::LibraryScanner::getLibraryType(libName) != arcade::LibraryInfo::Type::Display) {
        std::cerr << "Error: '" << argv[1] << "' not a graphical library" << std::endl;
        return 84;
    }

    auto core = std::make_unique<arcade::Core>(argv[1]);
    core->init();
    core->run();
    core->stop();
    return 0;
}