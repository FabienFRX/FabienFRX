/*
** EPITECH PROJECT, 2025
** Object addition/subtraction
** File description:
** float.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "float.h"
#include "new.h"
#include "raise.h"

typedef struct {
    Class base;
    float value;
} FloatClass;

static void Float_ctor(FloatClass *this, va_list *args)
{
    if (!this || !args)
        raise("Float_ctor: invalid args");
    ((FloatClass *)this)->value = (float)va_arg(*args, double);
}

static void Float_dtor(FloatClass *this)
{
    (void)this;
}

static char *Float_str(FloatClass *this)
{
    FloatClass *self = (FloatClass *)this;
    char *buffer = malloc(32);
    if (!buffer)
        raise("Out of memory");
    snprintf(buffer, 32, "<Float (%.2f)>", self->value);
    return buffer;
}

static Object *Float_add(const FloatClass *this, const FloatClass *other)
{
    const FloatClass *a = (const FloatClass *)this;
    const FloatClass *b = (const FloatClass *)other;
    return new(Float, a->value + b->value);
}

static Object *Float_sub(const FloatClass *this, const FloatClass *other)
{
    if (!this || !other)
        raise("Invalid argument for substraction");
    return new(Float, this->value - other->value);
}

Object *Float_mul(const FloatClass *this, const FloatClass *other)
{
    if (!this || !other)
        raise("Invalid argument for multiplication");
    return new(Float, this->value * other->value);
}

Object *Float_div(const FloatClass *this, const FloatClass *other)
{
    if (!this || !other)
        raise("Invalid argument for division");
    if (other->value == 0)
        raise("Division by zero");
    return new(Float, this->value / other->value);
}

bool Float_eq(const FloatClass *this, const FloatClass *other)
{
    if (!this || !other)
        raise("Invalid argument for equality");
    return (this->value == other->value);
}

bool Float_gt(const FloatClass *this, const FloatClass *other)
{
    if (!this || !other)
        raise("Invalid argument for greater");
    return (this->value > other->value);
}

bool Float_lt(const FloatClass *this, const FloatClass *other)
{
    if (!this || !other)
        raise("Invalid argument for lower");
    return (this->value < other->value);
}

static const FloatClass _description = {
    {
        .__size__ = sizeof(FloatClass),
        .__name__ = "Float",
        .__ctor__ = (ctor_t)&Float_ctor,
        .__dtor__ = (dtor_t)&Float_dtor,
        .__str__  = (to_string_t)&Float_str,
        .__add__  = (binary_operator_t)&Float_add,
        .__sub__  = (binary_operator_t)&Float_sub,
        .__mul__  = (binary_operator_t)&Float_mul,
        .__div__  = (binary_operator_t)&Float_div,
        .__eq__   = (binary_comparator_t)&Float_eq,
        .__gt__   = (binary_comparator_t)&Float_gt,
        .__lt__   = (binary_comparator_t)&Float_lt
    },
    .value = 0.0
};

const Class *Float = (const Class *)&_description;
