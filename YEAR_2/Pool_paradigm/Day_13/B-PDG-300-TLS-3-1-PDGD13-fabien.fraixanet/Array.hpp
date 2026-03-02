/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD13-fabien.fraixanet
** File description:
** Array
*/

#ifndef ARRAY_HPP_
    #define ARRAY_HPP_

    #include <iostream>
    #include <functional>

template <typename Type, std::size_t Size>
class Array {
    public:
    Type &operator[](std::size_t index)
    {
        if (index >= Size)
            throw std::out_of_range("Out of range");
        return m_data[index];
    }

    const Type &operator[](std::size_t index) const
    {
        if (index >= Size) {
            throw std::out_of_range("Out of range");
        }
        return m_data[index];
    }

    std::size_t size() const
    {
        return Size;
    }

    void forEach(const std::function<void(const Type &)> &task) const
    {
        for (const auto &elem : m_data) {
            task(elem);
        }
    }

    template <typename U>
    Array<U, Size> convert(const std::function<U(const Type &)> &converter) const {
        Array<U, Size> newArray;
        for (std::size_t i = 0; i < Size; ++i) {
            newArray[i] = converter(m_data[i]);
        }
        return newArray;
    }

    void print(std::ostream &os) const
    {
        os << "[";
        for (std::size_t i = 0; i < Size; ++i) {
            os << m_data[i];
            if (i < Size - 1) {
                os << ", ";
            }
        }
        os << "]";
    }

private:
    Type m_data[Size]{};
};

template <typename Type, std::size_t Size>
std::ostream &operator<<(std::ostream &os, const Array<Type, Size> &arr)
{
    arr.print(os);
    return os;

};

#endif /* !ARRAY_HPP_ */
