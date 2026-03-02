/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD10-fabien.fraixanet
** File description:
** FruitUtils
*/

#ifndef FRUITUTILS_HPP_
    #define FRUITUTILS_HPP_

    #include "FruitBox.hpp"
    #include "IFruit.hpp"
    #include "Lemon.hpp"

class FruitUtils {
public:
    static void sort(FruitBox &unsorted, FruitBox &lemon, FruitBox &citrus, FruitBox &berry);
    static FruitBox** pack(IFruit** fruits, unsigned int boxSize);
    static IFruit** unpack(FruitBox** fruitBoxes);
};

#endif /* !FRUITUTILS_HPP_ */
