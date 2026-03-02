/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD06-fabien.fraixanet
** File description:
** MyCat
*/

#include <iostream>
#include <fstream>
#include <string>

void MyCat(const std::string &fileName)
{
    std::ifstream file(fileName);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "MyCat: " << fileName << ": No such file or directory" << std::endl;
        exit(84);
    }

    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }

    file.close();
}

int main(int argc, char **argv)
{
    if (argc == 1) {
        std::string line;
        while (std::getline(std::cin, line)) {
            std::cout << line << std::endl;
        }
    } else {
        for (int i = 1; i < argc; ++i) {
            MyCat(argv[i]);
        }
    }
    return 0;
}
