/*
** EPITECH PROJECT, 2025
** Tekspice
** File description:
** Parser
*/

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <string>
#include <vector>
#include <unordered_map>
#include "Circuit.hpp"
#include "IComponent.hpp"

namespace nts {
    class Circuit;
}

class Parser {
public:
    explicit Parser(const std::string& filename);
    void parseFile(nts::Circuit& circuit) const;

    const std::vector<std::pair<std::string, std::string>>& getLinks() const { return m_links; }
    const std::unordered_map<std::string, std::string>& getComponents() const { return m_components; }

private:
    std::string m_filename;
    std::unordered_map<std::string, std::string> m_components; // name -> type
    std::vector<std::pair<std::string, std::string>> m_links; // "A:1" -> "B:2"

    void parseLine(const std::string& line, nts::Circuit& circuit) const;
};

#endif // PARSER_HPP_
