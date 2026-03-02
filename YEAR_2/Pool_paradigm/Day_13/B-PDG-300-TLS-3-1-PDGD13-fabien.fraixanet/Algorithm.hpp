/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD13-fabien.fraixanet
** File description:
** Algorithm
*/

#ifndef ALGORITHM_HPP_
    #define ALGORITHM_HPP_

    #include <iostream>

template <typename T>
void swap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
const T &min(const T &a, const T &b)
{
    if (b < a)
        return b;
    return a;
}

template <typename T>
const T &max(const T &a, const T &b)
{
    if (b < a) {
        return a;
    }
    return b;
}

template <typename T>
const T &clamp(const T &value, const T &minValue, const T &maxValue)
{
    if (value < minValue)
        return minValue;
    if (maxValue < value)
        return maxValue;
    return value;
}

#endif /* !ALGORITHM_HPP_ */
