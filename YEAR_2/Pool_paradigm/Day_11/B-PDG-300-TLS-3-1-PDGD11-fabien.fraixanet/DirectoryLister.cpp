/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD11-fabien.fraixanet
** File description:
** DirectoryLister
*/

#include "DirectoryLister.hpp"

DirectoryLister::DirectoryLister() : m_dir(nullptr), m_hidden(false)
{
}

DirectoryLister::DirectoryLister(const std::string& path, bool hidden) : m_dir(nullptr), m_hidden(hidden)
{
    open(path, hidden);
}

DirectoryLister::~DirectoryLister()
{
    if (m_dir) {
        closedir(m_dir);
    }
}

bool DirectoryLister::open(const std::string& path, bool hidden)
{
    if (m_dir) {
        closedir(m_dir);
    }
    m_dir = opendir(path.c_str());
    m_hidden = hidden;

    if (!m_dir) {
        perror(path.c_str());
        return false;
    }
    return true;
}

std::string DirectoryLister::get()
{
    if (!m_dir) {
        return "";
    }

    struct dirent* entry;
    while ((entry = readdir(m_dir)) != nullptr) {
        std::string filename = entry->d_name;

        if (!m_hidden && filename[0] == '.') {
            continue;
        }
        return filename;
    }
    return "";
}
