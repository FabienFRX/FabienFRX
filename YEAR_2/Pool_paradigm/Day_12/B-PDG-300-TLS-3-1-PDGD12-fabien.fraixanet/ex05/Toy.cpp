/*
** EPITECH PROJECT, 2025
** 7
** File description:
** Toy
*/

#include "Toy.hpp"

Toy::Error::Error() : type(UNKNOWN)
{
}

std::string Toy::Error::what() const
{
    if (type == PICTURE)
        return "bad new illustration";
    if (type == SPEAK)
        return "wrong mode";
    return "";
}

std::string Toy::Error::where() const
{
    if (type == PICTURE)
        return "setAscii";
    if (type == SPEAK)
        return "speak_es";
    return "";
}

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
    if (!m_picture.getPictureFromFile(file)) {
        m_LastError.type = Error::PICTURE;
        return false;
    }
    m_LastError.type = Error::UNKNOWN;
    return true;
}

std::string Toy::getAscii() const
{
    return m_picture.data;
}

void Toy::speak(const std::string &statement)
{
    std::cout << m_name << " \"" << statement << "\"" << std::endl;
}

bool Toy::speak_es(const std::string &statement)
{
    if (m_type != BUZZ) {
        m_LastError.type = Error::SPEAK;
        return false;
    }
    std::cout << "BUZZ: " << m_name << " senorita \"" << statement << "\" senorita" << std::endl;
    m_LastError.type = Error::UNKNOWN;
    return true;
}

Toy::Error Toy::getLastError() const
{
    return m_LastError;
}

Toy &Toy::operator<<(const std::string &data)
{
    m_picture.data = data;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Toy &toy)
{
    os << toy.getName() << std::endl
       << toy.getAscii() << std::endl;
    return os;
}
