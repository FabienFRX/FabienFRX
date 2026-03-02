/*
** EPITECH PROJECT, 2025
** Tekspice
** File description:
** Parser
*/

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "Parser.hpp"
#include "Components.hpp"
#include "ElementaryComponents.hpp"
#include "GateComponents.hpp"

Parser::Parser(const std::string& filename) : m_filename(filename) {}

void Parser::parseFile(nts::Circuit& circuit) const {
    std::ifstream file(m_filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Cannot open file " + m_filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        parseLine(line, circuit);
    }
}

void Parser::parseLine(const std::string& line, nts::Circuit& circuit) const {
    std::istringstream iss(line);
    std::string token;
    if (line.find(".chipsets:") != std::string::npos) {
        while (iss >> token) {
            if (token == "input") {
                std::string name;
                iss >> name;
                circuit.addComponent(name, new nts::InputComponent());
            } else if (token == "output") {
                std::string name;
                iss >> name;
                circuit.addComponent(name, new nts::OutputComponent());
            } else if (token == "4071") {
                std::string name;
                iss >> name;
                circuit.addComponent(name, new nts::Or4071Component());
            }
        }
    } else if (line.find(".links:") != std::string::npos) {
        while (iss >> token) {
            std::string from, to;
            std::size_t fromPin, toPin;
            std::replace(token.begin(), token.end(), ':', ' ');
            std::istringstream linkStream(token);
            linkStream >> from >> fromPin >> to >> toPin;
            nts::IComponent* fromComponent = circuit.getComponent(from);
            nts::IComponent* toComponent = circuit.getComponent(to);
            if (fromComponent && toComponent) {
                fromComponent->setLink(fromPin, *toComponent, toPin);
            } else {
                std::cerr << "Error: Invalid link between " << from << " and " << to << std::endl;
            }
        }
    }
}
