/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD11-fabien.fraixanet
** File description:
** UniquePointer
*/

#ifndef UNIQUEPOINTER_HPP
    #define UNIQUEPOINTER_HPP

    #include "IObject.hpp"

class UniquePointer {
public:
    UniquePointer();
    UniquePointer(IObject* ptr);
    ~UniquePointer();

    UniquePointer(const UniquePointer&) = delete;
    UniquePointer& operator=(const UniquePointer&) = delete;

    UniquePointer(UniquePointer&& other) noexcept;
    UniquePointer& operator=(UniquePointer&& other) noexcept;

    IObject* operator->() const;
    IObject& operator*() const;

    void reset(IObject* ptr = nullptr);
    void swap(UniquePointer& other);

private:
    IObject* m_ptr;
};

#endif /* !UNIQUEPOINTER_HPP_ */
