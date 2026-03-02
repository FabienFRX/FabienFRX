/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD13-fabien.fraixanet
** File description:
** Stack
*/

#ifndef STACK_HPP_
#define STACK_HPP_

#include <stack>
#include <stdexcept>

class Stack {
public:
    class Error : public std::exception {
    public:
        Error(const char *msg) : m_msg(msg)
        {
        }
        const char *what() const noexcept override
        {
            return m_msg;
        }
    private:
        const char *m_msg;
    };

    void push(double value);
    double pop();
    double top() const;
    void add();
    void sub();
    void mul();
    void div();

private:
    std::stack<double> m_stack;
};

#endif /* !STACK_HPP_ */
