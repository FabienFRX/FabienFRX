/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD07pm-fabien.fraixanet
** File description:
** KoalaBot
*/

#ifndef KOALABOT_HPP_
    #define KOALABOT_HPP_

    #include "Parts.hpp"

class KoalaBot {
public:
    KoalaBot(std::string serial = "Bob-01");
    bool status() const;
    void informations() const;
    void setParts(const Arms &arms);
    void setParts(const Legs &legs);
    void setParts(const Head &head);
    void swapParts(Arms &arms);
    void swapParts(Legs &legs);
    void swapParts(Head &head);

private:
    std::string m_serial;
    Arms m_arms;
    Legs m_legs;
    Head m_head;
};
#endif /* !KOALABOT_HPP_ */
