/*
** EPITECH PROJECT, 2025
** Tekspice
** File description:
** Loop
*/

#include "Loop.hpp"
#include <iostream>
#include <sstream>

Loop::Loop(nts::Circuit& circuit) : m_circuit(circuit), m_running(true) {}

void Loop::run() {
    std::string command;

    while (m_running) {
        std::cout << "> ";
        if (!std::getline(std::cin, command)) {
            m_running = false; // Gérer CTRL+D
            break;
        }
        processCommand(command);
    }
}

void Loop::processCommand(const std::string& command) {
    if (command == "exit") {
        m_running = false;
    } else if (command == "simulate") {
        m_circuit.simulate();
    } else if (command == "display") {
        m_circuit.display();
    } else {
        std::istringstream iss(command);
        std::string name;
        char equal;
        int value;
        if (iss >> name >> equal >> value && equal == '=') {
            m_circuit.setInputValue(name, static_cast<nts::Tristate>(value));
        } else {
            std::cerr << "Error: Invalid command." << std::endl;
        }
    }
}
