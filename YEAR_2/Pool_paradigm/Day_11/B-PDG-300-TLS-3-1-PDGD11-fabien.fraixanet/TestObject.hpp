/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD11-fabien.fraixanet
** File description:
** TestObject
*/

#ifndef TESTOBJECT_HPP_
    #define TESTOBJECT_HPP_

    #include <iostream>
    #include <string>
    #include "IObject.hpp"

class TestObject : public IObject {
private:
    std::string m_name;
public:
    TestObject(const std::string& name) : m_name(name) {
        std::cout << m_name << " is alive" << std::endl;
    }

    ~TestObject() override {
        std::cout << m_name << " is dead" << std::endl;
    }

    void touch() override {
        std::cout << m_name << " is touched" << std::endl;
    }
};

#endif /* !TESTOBJECT_HPP_ */
