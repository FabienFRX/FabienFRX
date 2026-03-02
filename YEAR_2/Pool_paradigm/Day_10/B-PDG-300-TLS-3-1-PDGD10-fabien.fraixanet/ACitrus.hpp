/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD10-fabien.fraixanet
** File description:
** ACitrus
*/

#ifndef ACITRUS_HPP_
    #define ACITRUS_HPP_

    #include "AFruit.hpp"

class ACitrus : public AFruit {
public:
    ACitrus(const std::string &name, unsigned int vitamins);
    ~ACitrus() override;

protected:
private:
};
#endif /* !ACITRUS_HPP_ */
