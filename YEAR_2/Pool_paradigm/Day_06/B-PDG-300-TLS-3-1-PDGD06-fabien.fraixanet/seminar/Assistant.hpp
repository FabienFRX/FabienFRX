/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD06-fabien.fraixanet
** File description:
** Assistant
*/

#ifndef ASSISTANT_HPP_
    #define ASSISTANT_HPP_

    #include <string>
    #include <iostream>
    #include "Student.hpp"

class Assistant {

    public:
        Assistant(int id);
        ~Assistant();

        void giveDrink(Student* student, const std::string& drink);
        std::string readDrink(const std::string& studentName);
        void helpStudent(Student* student);
        void timeCheck();

    private:
        int m_ID;
        bool m_isWorking;
};
#endif /* !ASSISTANT_HPP_ */
