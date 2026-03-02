/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD12-fabien.fraixanet
** File description:
** Toy
*/

#ifndef TOY_HPP_
    #define TOY_HPP_

    #include <iostream>
    #include "Picture.hpp"

class Toy {
public:
    enum ToyType {
        BASIC_TOY,
        ALIEN,
        BUZZ,
        WOODY,
    };

    Toy();
    Toy(ToyType type, const std::string &name, const std::string &file);

    ToyType getType() const;
    std::string getName() const;
    void setName(const std::string &name);
    bool setAscii(const std::string &file);
    std::string getAscii() const;

private:
    ToyType m_type;
    std::string m_name;
    Picture m_picture;
};
#endif /* !TOY_HPP_ */
