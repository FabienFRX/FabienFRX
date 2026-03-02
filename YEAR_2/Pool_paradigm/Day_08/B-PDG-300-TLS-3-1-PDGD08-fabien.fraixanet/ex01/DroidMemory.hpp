/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD08-fabien.fraixanet
** File description:
** DroidMemory
*/

#ifndef DROIDMEMORY_HPP
    #define DROIDMEMORY_HPP

    #include <iostream>
    #include <cstdlib>

    class DroidMemory {
    public:
        DroidMemory();
        DroidMemory(const DroidMemory& other);
        ~DroidMemory();

        size_t getFingerprint() const;
        size_t getExp() const;

        void setFingerprint(size_t fingerprint);
        void setExp(size_t exp);

        DroidMemory& operator=(const DroidMemory& other);
        DroidMemory& operator<<(const DroidMemory& other);
        DroidMemory& operator>>(DroidMemory& other);
        DroidMemory& operator+=(const DroidMemory& other);
        DroidMemory& operator+=(size_t exp);
        DroidMemory operator+(const DroidMemory& other) const;
        DroidMemory operator+(size_t exp) const;
        std::ostream& print(std::ostream& os) const;

    private:
        size_t m_Fingerprint;
        size_t m_Exp;
};

std::ostream& operator<<(std::ostream& os, const DroidMemory& droid);

#endif
