/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD11-fabien.fraixanet
** File description:
** SafeDirectoryLister
*/

#ifndef SAFEDIRECTORYLISTER_HPP_
    #define SAFEDIRECTORYLISTER_HPP_

    #include "IDirectoryLister.hpp"
    #include <dirent.h>
    #include <iostream>

class SafeDirectoryLister : public IDirectoryLister {
public:
    SafeDirectoryLister();
    SafeDirectoryLister(const std::string& path, bool hidden);
    ~SafeDirectoryLister();

    bool open(const std::string& path, bool hidden);
    std::string get() override;

    SafeDirectoryLister(const SafeDirectoryLister&) = delete;
    SafeDirectoryLister& operator=(const SafeDirectoryLister&) = delete;
    SafeDirectoryLister(SafeDirectoryLister&&) = delete;
    SafeDirectoryLister& operator=(SafeDirectoryLister&&) = delete;

private:
    DIR* m_dir;
    bool m_hidden;
};

#endif /* !SAFEDIRECTORYLISTER_HPP_ */
