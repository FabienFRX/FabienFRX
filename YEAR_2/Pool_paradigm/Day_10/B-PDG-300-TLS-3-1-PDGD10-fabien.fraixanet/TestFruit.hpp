/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD10-fabien.fraixanet
** File description:
** TestFruit
*/

#ifndef TESTFRUIT_HPP
#define TESTFRUIT_HPP

#include "IFruit.hpp"

class TestFruit : public IFruit {
public:
    explicit TestFruit(const std::string& name);
    ~TestFruit();

    std::string getName() const override;
    unsigned int getVitamins() const override;
    bool isPeeled() const override;
    void peel() override;

private:
    std::string m_name;
    int m_vitamins;
    bool m_isPeeled;
};

#endif // TESTFRUIT_HPP

