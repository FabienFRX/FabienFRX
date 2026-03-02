/*
** EPITECH PROJECT, 2025
** Day_09
** File description:
** Knight
*/

#ifndef KNIGHT_HPP
    #define KNIGHT_HPP

    #include "Peasant.hpp"

    class Knight : virtual public Peasant {
    public:
        Knight(const std::string &name, int power);
        ~Knight();

        virtual int attack() override;
        int special() override;
        void rest() override;

    protected:
    private:

    };

#endif // KNIGHT_HPP