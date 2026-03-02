/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD11-fabien.fraixanet
** File description:
** UniquePointer
*/

#include "UniquePointer.hpp"

UniquePointer::UniquePointer() : m_ptr(nullptr)
{
}

UniquePointer::UniquePointer(IObject* ptr) : m_ptr(ptr)
{
}

UniquePointer::~UniquePointer()
{
    delete m_ptr;
}

UniquePointer::UniquePointer(UniquePointer&& other) noexcept : m_ptr(other.m_ptr)
{
    other.m_ptr = nullptr;
}

UniquePointer& UniquePointer::operator=(UniquePointer&& other) noexcept
{
    if (this != &other) {
        delete m_ptr;
        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
    }
    return *this;
}

IObject* UniquePointer::operator->() const
{
    return m_ptr;
}

IObject& UniquePointer::operator*() const
{
    return *m_ptr;
}

void UniquePointer::reset(IObject* ptr)
{
    if (m_ptr != ptr) {
        delete m_ptr;
        m_ptr = ptr;
    }
}

void UniquePointer::swap(UniquePointer& other)
{
    IObject* temp = m_ptr;
    m_ptr = other.m_ptr;
    other.m_ptr = temp;
}
