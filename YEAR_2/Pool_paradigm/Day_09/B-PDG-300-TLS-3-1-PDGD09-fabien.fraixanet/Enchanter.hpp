/*
** EPITECH PROJECT, 2025
** Day_09
** File description:
** Enchanter
*/

#ifndef ENCHANTER_HPP_
    #define ENCHANTER_HPP_

    #include "Peasant.hpp"

    class Enchanter : virtual public Peasant {
    public:
        Enchanter(const std::string &name, int power);
        ~Enchanter();

        int attack() override;
        int special() override;
        void rest() override;

    protected:
    private:

    };

#endif /* !ENCHANTER_HPP_ */
