/*
** EPITECH PROJECT, 2025
** Tekspice
** File description:
** Main
*/

#include <iostream>
#include "Circuit.hpp"
#include "Loop.hpp"
#include "Parser.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <circuit_file>" << std::endl;
        return 84;
    }

    try {
        Parser parser(argv[1]);
        nts::Circuit circuit(parser);
        Loop loop(circuit);
        loop.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }

    return 0;
}
