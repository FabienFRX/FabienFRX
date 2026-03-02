/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sat, Mar, 2025                                                     *
 * Title           - B-OOP-400-TLS-4-1-arcade-nolan.dumoulin [WSL: Debian]              *
 * Description     -                                                                    *
 *     LibraryScanner                                                                   *
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

#include "LibraryScanner.hpp"

namespace arcade {
    std::vector<LibraryInfo> LibraryScanner::scanLibraries()
    {
        std::vector<LibraryInfo> libraries;
        DIR *dir = opendir("./lib");
        if (!dir) {
            throw LibraryLoadException("Failed to open .lib directory");
        }
        struct dirent *entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_DIR) {
                continue;
            }
            std::string filename(entry->d_name);
            if (filename.size() < 3 || filename.substr(filename.size() - 3) != ".so") {
                continue;
            }
            std::string path = "./lib/" + filename;
            std::string name = getLibraryName(filename);
            LibraryInfo::Type type = getLibraryType(name);
            libraries.push_back({path, name, type});
        }
        closedir(dir);
        return libraries;
    }

    std::string LibraryScanner::getLibraryName(const std::string &path)
    {
        size_t pos = path.find_last_of('/');
        std::string filename = (pos == std::string::npos) ? path : path.substr(pos + 1);

        if (filename.size() > 7 && filename.substr(0, 7) == "arcade_") {
            filename = filename.substr(7);
        }
        if (filename.size() > 3 && filename.substr(filename.size() - 3) == ".so") {
            filename = filename.substr(0, filename.size() - 3);
        }
        return filename;
    }

    LibraryInfo::Type LibraryScanner::getLibraryType(const std::string &name)
    {
        static const std::vector<std::string> displayModules = {
            "ncurses", "sdl2", "sfml", "ndk++", "aalib", "libcaca", "allegro5",
            "xlib", "gtk+", "irrlicht", "opengl", "vulkan", "qt5"
        };
        static const std::vector<std::string> gameModules = {
            "snake", "nibbler", "pacman", "qix", "centipede", "solarfox", "minesweeper"
        };
        if (std::find(displayModules.begin(), displayModules.end(), name) != displayModules.end()) {
            return LibraryInfo::Type::Display;
        }
        if (std::find(gameModules.begin(), gameModules.end(), name) != gameModules.end()) {
            return LibraryInfo::Type::Game;
        }
        return LibraryInfo::Type::Unknown;
    }

    bool LibraryScanner::isValidLibrary(const std::string &path)
    {
        void *handle = dlopen(path.c_str(), RTLD_LAZY);
        if (!handle) {
            return false;
        }
        dlclose(handle);
        return true;
    }
}
