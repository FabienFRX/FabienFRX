/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGRUSH2-andi.berard [WSL: Ubuntu-22.04]
** File description:
** new
*/

#include <stdlib.h>
#include <stdarg.h>
#include "new.h"
#include "raise.h"

Object *new(const Class *class, ...)
{
    va_list args;

    if (!class)
        raise("Class is NULL");
    Object *obj = malloc(class->__size__);
    if (!obj)
        raise("Out of memory");
    memcpy(obj, class, sizeof(Class));
    if (class->__ctor__) {
        va_start(args, class);
        class->__ctor__(obj, &args);
        va_end(args);
    }
    return obj;
}

Object *va_new(const Class *class, va_list *ap)
{
    if (!class)
        raise("Class is NULL");
    Object *obj = malloc(class->__size__);
    if (!obj)
        raise("Out of memory");
    memcpy(obj, class, sizeof(Class));
    if (class->__ctor__)
        class->__ctor__(obj, ap);
    return obj;
}

void delete(Object *ptr)
{    
    if (!ptr)
        raise("object is NULL");
    Class *class =  malloc(sizeof(Class));
    if (!class) {
        raise("class is NULL");
    }
    memcpy(class, ptr, sizeof(Class));
    if (class->__dtor__)
        class->__dtor__(ptr);
    free(class);
    free(ptr);
}
