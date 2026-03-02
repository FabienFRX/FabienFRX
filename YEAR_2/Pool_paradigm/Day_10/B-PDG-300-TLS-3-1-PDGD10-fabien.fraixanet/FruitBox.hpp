/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD10-fabien.fraixanet
** File description:
** FruitBox
*/

#ifndef FRUITBOX_HPP
    #define FRUITBOX_HPP

    #include "IFruit.hpp"
    #include <vector>
    #include <iostream>
    #include <string>

class FruitBox {
public:
    FruitBox(unsigned int size);
    ~FruitBox();

    unsigned int getSize() const;
    unsigned int nbFruits() const;
    bool pushFruit(IFruit* fruit);
    IFruit* popFruit();

    std::string toString() const;

    FruitBox(const FruitBox&) = delete;
    FruitBox& operator=(const FruitBox&) = delete;

private:
    unsigned int m_size;
    std::vector<IFruit*> m_fruits;
};

std::ostream& operator<<(std::ostream& os, const FruitBox& box);

#endif // FRUITBOX_HPP

