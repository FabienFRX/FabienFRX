/*
** EPITECH PROJECT, 2025
** 7
** File description:
** Toy
*/

#include "Toy.hpp"

Toy::Toy() : m_type(BASIC_TOY), m_name("toy"), m_picture("")
{
}

Toy::Toy(ToyType type, const std::string &name, const std::string &file)
    : m_type(type), m_name(name), m_picture(file)
{
    m_picture.getPictureFromFile(file);
}

Toy::~Toy()
{
}

Toy::ToyType Toy::getType() const
{
    return m_type;
}

std::string Toy::getName() const
{
    return m_name;
}

void Toy::setName(const std::string &name)
{
    m_name = name;
}

bool Toy::setAscii(const std::string &file)
{
    return m_picture.getPictureFromFile(file);
}

std::string Toy::getAscii() const
{
    return m_picture.data;
}

void Toy::speak(const std::string &statement)
{
    std::cout << m_name << " \"" << statement << "\"" << std::endl;
}