/*
** EPITECH PROJECT, 2025
** Tekspice
** File description:
** Loop
*/

#ifndef LOOP_HPP_
#define LOOP_HPP_

#include "Circuit.hpp"

class Loop {
public:
    explicit Loop(nts::Circuit& circuit);
    void run();

private:
    nts::Circuit& m_circuit;
    bool m_running;

    void processCommand(const std::string& command);
};

#endif // LOOP_HPP_
