/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD12-fabien.fraixanet
** File description:
** Woody
*/

#ifndef WOODY_HPP_
    #define WOODY_HPP_

    #include "Toy.hpp"

class Woody : public Toy {
public:
    Woody(const std::string &name, const std::string &file = "woody.txt");
    void speak(const std::string &statement) override;
};

#endif /* !WOODY_HPP_ */
