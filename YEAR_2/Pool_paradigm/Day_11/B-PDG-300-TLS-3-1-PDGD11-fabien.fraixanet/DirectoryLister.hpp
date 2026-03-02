/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD11-fabien.fraixanet
** File description:
** DirectoryLister
*/

#ifndef DIRECTORYLISTER_HPP_
    #define DIRECTORYLISTER_HPP_

    #include "IDirectoryLister.hpp"
    #include <dirent.h>
    #include <iostream>

class DirectoryLister : public IDirectoryLister {
public:
    DirectoryLister();
    DirectoryLister(const std::string& path, bool hidden);
    ~DirectoryLister();

    bool open(const std::string& path, bool hidden) override;
    std::string get() override;

    DirectoryLister(const DirectoryLister&) = delete;
    DirectoryLister& operator=(const DirectoryLister&) = delete;
    DirectoryLister(DirectoryLister&&) = delete;
    DirectoryLister& operator=(DirectoryLister&&) = delete;

private:
    DIR* m_dir;
    bool m_hidden;
};

#endif /* !DIRECTORYLISTER_HPP_ */
