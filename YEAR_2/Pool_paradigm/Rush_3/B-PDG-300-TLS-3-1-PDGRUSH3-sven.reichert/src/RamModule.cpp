/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sun, Feb, 2025                                                     *
 * Title           - B-PDG-300-TLS-3-1-PDGRUSH3-sven.reichert [WSL: Debian]             *
 * Description     -                                                                    *
 *     RamModule                                                                        *
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

#include "../include/RamModule.hpp"

Krell::RAMModule::RAMModule() {
    readMemInfo();
}

void Krell::RAMModule::update() {
    readMemInfo();
}

void Krell::RAMModule::readMemInfo() {
    std::ifstream file("/proc/meminfo");
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (line.find("MemTotal") != std::string::npos) {
                std::istringstream iss(line);
                std::string key;
                iss >> key >> totalRAM;
            } else if (line.find("MemFree") != std::string::npos) {
                std::istringstream iss(line);
                std::string key;
                iss >> key >> freeRAM;
            }
        }
        file.close();
    }
}

void Krell::RAMModule::render(IDisplay &display) {
    this->update();
    std::string data = "Total RAM: " + totalRAM + " kB\nFree RAM: " + freeRAM + " kB";
    display.draw(data);
}

std::string Krell::RAMModule::getData() const {
    return "Total RAM: " + totalRAM + " kB\nFree RAM: " + freeRAM + " kB";
}