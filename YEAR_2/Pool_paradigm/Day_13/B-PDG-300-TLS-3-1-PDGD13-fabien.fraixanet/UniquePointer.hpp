/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD13-fabien.fraixanet
** File description:
** UniquePointer
*/

#ifndef UNIQUEPOINTER_HPP_
    #define UNIQUEPOINTER_HPP_

    #include <iostream>

template <typename T>
class UniquePointer {
public:
    UniquePointer(T* ptr = nullptr) : m_ptr(ptr) {}
    ~UniquePointer()
    {
        delete m_ptr;
        m_ptr = nullptr;
    }

    UniquePointer(const UniquePointer&) = delete;
    UniquePointer& operator=(const UniquePointer&) = delete;
    UniquePointer& operator=(T* ptr)
    {
        if (m_ptr != ptr) {
            delete m_ptr;
            m_ptr = ptr;
        }
        return *this;
    }

    void reset()
    {
        delete m_ptr;
        m_ptr = nullptr;
    }

    T* get() const
    {
        return m_ptr;
    }

    T* operator->()
    {
        return m_ptr;
    }

    const T* operator->() const
    {
        return m_ptr;
    }

    T& operator*()
    {
        return *m_ptr;
    }

    const T& operator*() const
    {
        return *m_ptr;
    }

private:
    T* m_ptr;
};

#endif /* !UNIQUEPOINTER_HPP_ */
