/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD10-fabien.fraixanet
** File description:
** Test_main
*/

#include <iostream>
#include <string>
#include "ABerry.hpp"
#include "ACitrus.hpp"
#include "AFruit.hpp"
#include "Almond.hpp"
#include "ANut.hpp"
#include "IFruit.hpp"
#include "Lemon.hpp"
#include "Orange.hpp"
#include "Strawberry.hpp"
int main(void)
{
    Orange o;
    Strawberry s;
    const Almond a;
    IFruit& f = o;

    std::cout << o.getName() << ": " << o.getVitamins() << " vitamins, Peeled: " << (o.isPeeled() ? "true" : "false") << std::endl;
    std::cout << "Expected: orange: 0 vitamins, Peeled: false" << std::endl;

    std::cout << s << std::endl;
    std::cout << "Expected: [name: strawberry, vitamins: 6, peeled: true]" << std::endl;

    std::cout << a << std::endl;
    std::cout << "Expected: [name: almond, vitamins: 0, peeled: false]" << std::endl;

    o.peel();
    std::cout << f << std::endl;
    std::cout << "Expected: [name: orange, vitamins: 7, peeled: true]" << std::endl;

    std::cout << o.getName() << ": Peeled: " << (o.isPeeled() ? "true" : "false") << std::endl;
    std::cout << "Expected: orange: Peeled: true" << std::endl;

    return 0;
}