/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD11-fabien.fraixanet
** File description:
** IDirectoryLister
*/

#include "IDirectoryLister.hpp"
#include <cstring>
#include <errno.h>

IDirectoryLister::OpenFailureException::OpenFailureException(const std::string&)
    : m_message(std::strerror(errno))
{
}

const char* IDirectoryLister::OpenFailureException::what() const noexcept
{
    return m_message.c_str();
}

const char* IDirectoryLister::NoMoreFileException::what() const noexcept
{
    return "End of stream";
}
