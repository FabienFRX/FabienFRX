/*
** EPITECH PROJECT, 2025
** Object addition/subtraction
** File description:
** char.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "char.h"
#include "new.h"
#include "raise.h"

typedef struct {
    Class base;
    char c;
} CharClass;

static void Char_ctor(CharClass *this, va_list *args)
{
    if (!this || !args)
        raise("Char_ctor: invalid args");
    ((CharClass *)this)->c = (char)va_arg(*args, int);
}

static void Char_dtor(CharClass *this)
{
    (void)this;
}

static char *Char_str(CharClass *this)
{
    char *str = NULL;

    if (this->c >= 32 && this->c <= 126) {
        str = malloc(snprintf(NULL, 0, "<Char (%c)>", this->c) + 1);
        if (!str)
            raise("Out of memory");
        sprintf(str, "<Char (%c)>", this->c);
    } else {
        str = malloc(snprintf(NULL, 0, "<Char (%c)>", this->c) + 1);
        if (!str)
            raise("Out of memory");
        sprintf(str, "<Char (%d)>", (int)this->c);
    }
    return str;
}

static Object *Char_add(const CharClass *this, const CharClass *other)
{
    if (!this || !other)
        raise("Invalid argument for addition");
    return new(Char, (char)(this->c + other->c));
}

static Object *Char_sub(const CharClass *this, const CharClass *other)
{
    if (!this || !other)
        raise("Invalid argument for substraction");
    return new(Char, (char)(this->c - other->c));
}

Object *Char_mul(const CharClass *this, const CharClass *other)
{
    if (!this || !other)
        raise("Invalid argument for multiplication");
    return new(Char, (char)(this->c * other->c));
}

Object *Char_div(const CharClass *this, const CharClass *other)
{
    if (!this || !other)
        raise("Invalid argument for division");
    if (other->c == 0)
        raise("Division by zero");
    return new(Char, (char)(this->c / other->c));
}

bool Char_eq(const CharClass *this, const CharClass *other)
{
    if (!this || !other)
        raise("Invalid argument for equality");
    return (this->c == other->c);
}

bool Char_gt(const CharClass *this, const CharClass *other)
{
    if (!this || !other)
        raise("Invalid argument for greater");
    return (this->c > other->c);
}

bool Char_lt(const CharClass *this, const CharClass *other)
{
    if (!this || !other)
        raise("Invalid argument for lower");
    return (this->c < other->c);
}

static const CharClass _description = {
    {
        .__size__ = sizeof(CharClass),
        .__name__ = "Char",
        .__ctor__ = (ctor_t)&Char_ctor,
        .__dtor__ = (dtor_t)&Char_dtor,
        .__str__  = (to_string_t)&Char_str,
        .__add__  = (binary_operator_t)&Char_add,
        .__sub__  = (binary_operator_t)&Char_sub,
        .__mul__  = (binary_operator_t)&Char_mul,
        .__div__  = (binary_operator_t)&Char_div,
        .__eq__   = (binary_comparator_t)&Char_eq,
        .__gt__   = (binary_comparator_t)&Char_gt,
        .__lt__   = (binary_comparator_t)&Char_lt
    },
    .c = '\0'
};

const Class *Char = (const Class *)&_description;