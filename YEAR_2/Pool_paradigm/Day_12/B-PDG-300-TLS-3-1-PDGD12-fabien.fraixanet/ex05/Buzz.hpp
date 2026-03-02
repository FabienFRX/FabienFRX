/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD12-fabien.fraixanet
** File description:
** Buzz
*/

#ifndef BUZZ_HPP_
    #define BUZZ_HPP_

    #include "Toy.hpp"

class Buzz : public Toy {
public:
    Buzz(const std::string &name, const std::string &file = "buzz.txt");
    void speak(const std::string &statement) override;
    bool speak_es(const std::string &statement) override;
};

#endif /* !BUZZ_HPP_ */
