/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD06-fabien.fraixanet
** File description:
** Student
*/

#ifndef STUDENT_HPP_
    #define STUDENT_HPP_

    #include <string>

class Student {

    public:
        Student(const std::string& name);
        ~Student();

        bool learn(const std::string& text);
        void drink(const std::string& drink);
        std::string getName() const;

    private:
        std::string m_name;
        int m_energy;
};
#endif /* !STUDENT_HPP_ */
