/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD10-fabien.fraixanet
** File description:
** AFruit
*/

#ifndef AFRUIT_HPP_
    #define AFRUIT_HPP_

    #include <string>
    #include "IFruit.hpp"

class AFruit : public IFruit {

public:
    AFruit(const std::string &name, unsigned int vitamins);
    ~AFruit();

    unsigned int getVitamins() const;
    std::string getName() const;
    bool isPeeled() const;
    void peel();

protected:
    std::string m_name;
    unsigned int m_vitamins = 0;
    bool m_peeled;
};
#endif /* !AFRUIT_HPP_ */
