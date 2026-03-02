/*
** EPITECH PROJECT, 2025
** Tekspice
** File description:
** Circuit
*/

#ifndef CIRCUIT_HPP_
#define CIRCUIT_HPP_

#include <vector>
#include <string>
#include <iostream>
#include "IComponent.hpp"
#include "Parser.hpp"

class Parser;

namespace nts {
    class Circuit {
    public:
        explicit Circuit(const Parser& parser);
        ~Circuit();

        void simulate();
        void display() const;
        void setInputValue(const std::string& name, nts::Tristate value);

        void addComponent(const std::string& name, IComponent* component);
        IComponent* getComponent(const std::string& name) const;

    private:
        std::vector<std::pair<std::string, IComponent*>> m_components; // Remplace map
        std::size_t m_tick;
    };
}

#endif // CIRCUIT_HPP_
