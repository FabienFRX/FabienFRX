#include "CommandParser.hpp"
#include <iostream>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    try {
        CommandParser parser;
        parser.run();
    } catch (const std::exception &e) {
        std::cerr << "CRITICAL ERROR: " << e.what() << std::endl;
        return 84;
    }

    return 0;
}