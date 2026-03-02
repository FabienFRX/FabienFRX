/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD13-fabien.fraixanet
** File description:
** main
*/

#include "Algorithm.hpp"
#include "Array.hpp"
#include "Stack.hpp"
#include "UniquePointer.hpp"

int main(void)
{
    Stack stack;

    stack.push(42.21);
    stack.push(84.42);
    stack.push(21.12);
    stack.add();
    stack.div();

    try {
        stack.mul();
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << stack.top() << std::endl;
    return 0;
}