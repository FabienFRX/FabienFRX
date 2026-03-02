/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD13-fabien.fraixanet
** File description:
** Stack
*/

#include "Stack.hpp"

void Stack::push(double value)
{
    m_stack.push(value);
}

double Stack::pop()
{
    if (m_stack.empty())
        throw Error("Empty stack");
    double value = m_stack.top();
    m_stack.pop();
    return value;
}

double Stack::top() const
{
    if (m_stack.empty())
        throw Error("Empty stack");
    return m_stack.top();
}

void Stack::add()
{
    if (m_stack.size() < 2)
        throw Error("Not enough operands");
    double b = pop();
    double a = pop();
    push(a + b);
}

void Stack::sub()
{
    if (m_stack.size() < 2)
        throw Error("Not enough operands");
    double b = pop();
    double a = pop();
    push(b - a);
}

void Stack::mul()
{
    if (m_stack.size() < 2)
        throw Error("Not enough operands");
    double b = pop();
    double a = pop();
    push(a * b);
}

void Stack::div()
{
    if (m_stack.size() < 2)
        throw Error("Not enough operands");
    double b = pop();
    if (b == 0)
        throw Error("Division by zero");
    double a = pop();
    push(b / a);
}
