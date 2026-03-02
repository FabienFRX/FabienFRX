/*
** EPITECH PROJECT, 2025
** Object addition/subtraction
** File description:
** int.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "int.h"
#include "new.h"
#include "raise.h"

typedef struct {
    Class base;
    int value;
} IntClass;

static void Int_ctor(IntClass *this, va_list *args)
{
    if (!this || !args)
        raise("Int_ctor: invalid args");
    ((IntClass *)this)->value = va_arg(*args, int);
}

static void Int_dtor(IntClass *this)
{
    (void)this;
}

static char *Int_str(IntClass *this)
{
    char *str = NULL;
    
    if (!this) {
        raise("Invalid arguments for string");
    }
    str = malloc(sizeof(char) * snprintf(NULL, 0, "<Int (%d)>", this->value) + 1);

    sprintf(str, "<Int (%d)>", this->value);
    return str;
}

static Object *Int_add(const IntClass *this, const IntClass *other)
{
    if (!this || !other) {
        raise("Invalid arguments for addition");
    }
    return new(Int, this->value + other->value);
}

static Object *Int_sub(const IntClass *this, const IntClass *other)
{
    if (!this || !other) {
        raise("Invalid arguments for subtraction");
    }
    return new(Int, this->value - other->value);
}

Object *Int_mul(const IntClass *this, const IntClass *other)
{
        if (!this || !other) {
        raise("Invalid arguments for multiplication");
    }
    return new(Int, this->value * other->value);
}

Object *Int_div(const IntClass *this, const IntClass *other)
{
    if (!this || !other) {
        raise("Invalid arguments for division");
    }
    if (other->value == 0)
        raise("Division by zero");
    return new(Int, this->value / other->value);
}

bool Int_eq(const IntClass *this, const IntClass *other)
{
    if (!this || !other) {
        raise("Invalid arguments for equation");
    }
    return (this->value == other->value);
}

bool Int_gt(const IntClass *this, const IntClass *other)
{
    if (!this || !other) {
        raise("Invalid arguments for greater");
    }
    return (this->value > other->value);
}

bool Int_lt(const IntClass *this, const IntClass *other)
{
    if (!this || !other) {
        raise("Invalid arguments for lower");
    }
    return (this->value < other->value);
}

static const IntClass _description = {
    {
        .__size__ = sizeof(IntClass),
        .__name__ = "Int",
        .__ctor__ = (ctor_t)&Int_ctor,
        .__dtor__ = (dtor_t)&Int_dtor,
        .__str__  = (to_string_t)&Int_str,
        .__add__  = (binary_operator_t)&Int_add,
        .__sub__  = (binary_operator_t)&Int_sub,
        .__mul__  = (binary_operator_t)&Int_mul,
        .__div__  = (binary_operator_t)&Int_div,
        .__eq__   = (binary_comparator_t)&Int_eq,
        .__gt__   = (binary_comparator_t)&Int_gt,
        .__lt__   = (binary_comparator_t)&Int_lt
    },
    .value = 0
};

const Class *Int = (const Class *)&_description;
