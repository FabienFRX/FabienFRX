/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07pm-fabien.fraixanet
** File description:
** Parts
*/

#ifndef PARTS_HPP_
    #define PARTS_HPP_

    #include <iostream>
    #include <string>

class Arms {
public:
    Arms(std::string serial = "A-01", bool functional = true);
    bool isFunctional() const;
    std::string serial() const;
    void informations() const;

private:
    std::string m_serial;
    bool m_functional;
};

class Legs {
public:
    Legs(std::string serial = "L-01", bool functional = true);
    bool isFunctional() const;
    std::string serial() const;
    void informations() const;

private:
    std::string m_serial;
    bool m_functional;
};

class Head {
public:
    Head(std::string serial = "H-01", bool functional = true);
    bool isFunctional() const;
    std::string serial() const;
    void informations() const;

private:
    std::string m_serial;
    bool m_functional;
};

#endif /* !PARTS_HPP_ */
