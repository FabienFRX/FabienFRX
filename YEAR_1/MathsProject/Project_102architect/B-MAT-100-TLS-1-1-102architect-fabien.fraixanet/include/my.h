/*
** EPITECH PROJECT, 2023
** my.h
** File description:
** azerty
*/

#ifndef MY_H_
#define MY_H_
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#define M_PI 3.14159265358979323846

typedef struct
{
    float matrix[3][3];
}TransformationMatrix;

typedef struct
{
    float x;
    float y;
    float w;
} Point;

typedef struct
{
    float tx;
    float ty;
} Translation;

typedef struct
{
    float sx;
    float sy;
} Scaling;

typedef struct
{
    float angleInDegrees;
} Rotation;

typedef struct
{
    float angleInDegrees02;
} Reflection;

void display_h(void);
void xMatrix(Point *p, float matrix[3][3]);
void display_result(float *coord, float matrix [3][3]);
float degreesToRadians(float degrees);
TransformationMatrix translate(Point *p, Translation trans);
TransformationMatrix scale(Point *point, Scaling scale);
TransformationMatrix rotate(Point *point, Rotation ruota);
TransformationMatrix reflect(Point *point, Reflection rifletti);
#endif /* !MY_H_ */
