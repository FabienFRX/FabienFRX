/*
** EPITECH PROJECT, 2025
** Paradigms Seminar
** File description:
** Exercice 02
*/

#include <stdio.h>
#include "point.h"
#include "new.h"

typedef struct
{
    Class   base;
    int     x, y;
}   PointClass;

static void Point_ctor(PointClass *this, va_list *args)
{
    /* Fill this function for exercice 02 */
    this->x = va_arg(*args, int);
    this->y = va_arg(*args, int);
}

static void Point_dtor(PointClass *this)
{
    /* Fill this function for exercice 02 */
    if (!this) {
        raise("CLASS DEJA DESTROY !!");
    }
}

// Create additional functions here
static char *Point_str(PointClass *this)
{
    char *str = malloc(sizeof(char) * snprintf(NULL, 0, "<Point (%d, %d)>", this->x, this->y) + 1);

    sprintf(str, "<Point (%d, %d)>", this->x, this->y);
    return str;
}

static Object *Point_add(const PointClass *this, const PointClass *other)
{
    if (!this || !other) {
        raise("Invalid arguments for addition");
    }

    return new(Point, this->x + other->x, this->y + other->y);
}

static Object *Point_sub(const PointClass *this, const PointClass *other)
{
    if (!this || !other) {
        raise("Invalid arguments for subtraction");
    }

    return new(Point, this->x - other->x, this->y - other->y);
}

static const PointClass _description = {
    {   /* Class struct */
        .__size__ = sizeof(PointClass),
        .__name__ = "Point",
        .__ctor__ = (ctor_t)&Point_ctor,
        .__dtor__ = (dtor_t)&Point_dtor,
        .__str__ = (to_string_t)&Point_str,
        .__add__ = (binary_operator_t)&Point_add,
        .__sub__ = (binary_operator_t)&Point_sub,
        .__mul__ = NULL,
        .__div__ = NULL,
        .__eq__ = NULL,
        .__gt__ = NULL,
        .__lt__ = NULL
    },
    .x = 0,
    .y = 0
};

const Class   *Point = (const Class *)&_description;
