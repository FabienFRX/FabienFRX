/*
** EPITECH PROJECT, 2025
** Day_09
** File description:
** Peasant
*/

#ifndef PEASANT_HPP
    #define PEASANT_HPP

    #include <string>
    #include <iostream>
    #include "ICharacter.hpp"

    class Peasant : virtual public ICharacter {
    public:
        Peasant(const std::string &name, int power);
        ~Peasant();

        const std::string &getName() const;
        int getPower() const;
        int getHp() const;

        virtual int attack() override;
        virtual int special() override;
        virtual void rest() override;
        virtual void damage(int damage) override;

    protected:
        std::string m_name;
        int m_power;
        int m_hp;

        void capValues();
};

#endif // PEASANT_HPP

