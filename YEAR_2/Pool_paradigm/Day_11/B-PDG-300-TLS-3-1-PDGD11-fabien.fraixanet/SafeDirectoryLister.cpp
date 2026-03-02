/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD11-fabien.fraixanet
** File description:
** SafeDirectoryLister
*/

#include "SafeDirectoryLister.hpp"
#include "IDirectoryLister.hpp"

SafeDirectoryLister::SafeDirectoryLister() : m_dir(nullptr), m_hidden(false)
{
}

SafeDirectoryLister::SafeDirectoryLister(const std::string& path, bool hidden) : m_dir(nullptr), m_hidden(hidden)
{
    open(path, hidden);
}

SafeDirectoryLister::~SafeDirectoryLister()
{
    if (m_dir) {
        closedir(m_dir);
    }
}

bool SafeDirectoryLister::open(const std::string& path, bool hidden)
{
    if (m_dir) {
        closedir(m_dir);
    }
    m_dir = opendir(path.c_str());
    m_hidden = hidden;

    if (!m_dir) {
        throw OpenFailureException(path);
    }
    return true;
}

std::string SafeDirectoryLister::get()
{
    if (!m_dir) {
        throw NoMoreFileException();
    }

    struct dirent* entry;
    while ((entry = readdir(m_dir)) != nullptr) {
        std::string filename = entry->d_name;

        if (!m_hidden && filename[0] == '.') {
            continue;
        }
        return filename;
    }

    throw NoMoreFileException();
}
