/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD06-fabien.fraixanet
** File description:
** Dean
*/

#ifndef DEAN_HPP_
    #define DEAN_HPP_

    #include <string>
    #include <iostream>
    #include "Student.hpp"

class Dean {

    public:
        Dean(const std::string& name);
        ~Dean();

        void teachStudent(Student* student, const std::string& lesson);
        void timeCheck();

    private:
        std::string m_name;
        bool m_isWorking;
};
#endif /* !DEAN_HPP_ */
