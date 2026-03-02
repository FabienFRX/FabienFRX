/*
** EPITECH PROJECT, 2025
** Tekspice
** File description:
** Circuit
*/

#include "Circuit.hpp"
#include "Components.hpp"

nts::Circuit::Circuit(const Parser& parser) : m_tick(0) {
    parser.parseFile(*this);
}

nts::Circuit::~Circuit() {
    for (auto& component : m_components) {
        delete component.second;
    }
    m_components.clear();
}

void nts::Circuit::simulate() {
    m_tick++;
    for (auto& component : m_components) {
        component.second->simulate(m_tick);
    }
}

void nts::Circuit::display() const {
    std::cout << "tick: " << m_tick << std::endl;
    std::cout << "input(s):" << std::endl;
    for (const auto& component : m_components) {
        if (dynamic_cast<InputComponent*>(component.second)) {
            nts::Tristate state = component.second->compute(1);
            std::cout << component.first << ": "
                      << (state == nts::True ? "1" : (state == nts::False ? "0" : "U")) << std::endl;
        }
    }
    std::cout << "output(s):" << std::endl;
    for (const auto& component : m_components) {
        if (dynamic_cast<OutputComponent*>(component.second)) {
            nts::Tristate state = component.second->compute(1);
            std::cout << component.first << ": "
                      << (state == nts::True ? "1" : (state == nts::False ? "0" : "U")) << std::endl;
        }
    }
}

void nts::Circuit::setInputValue(const std::string& name, nts::Tristate value) {
    for (auto& component : m_components) {
        if (component.first == name) {
            if (auto inputComponent = dynamic_cast<InputComponent*>(component.second)) {
                inputComponent->setValue(value);
                inputComponent->simulate(0);
            } else {
                std::cerr << "Error: Component " << name << " is not an input component." << std::endl;
            }
            return;
        }
    }
    std::cerr << "Error: Input " << name << " not found." << std::endl;
}

void nts::Circuit::addComponent(const std::string& name, IComponent* component) {
    for (const auto& comp : m_components) {
        if (comp.first == name) {
            std::cerr << "Error: Component " << name << " already exists." << std::endl;
            return;
        }
    }
    m_components.emplace_back(name, component);
}

nts::IComponent* nts::Circuit::getComponent(const std::string& name) const {
    for (const auto& component : m_components) {
        if (component.first == name) {
            return component.second;
        }
    }
    return nullptr;
}
