/*
** EPITECH PROJECT, 2023
** 102Architect
** File description:
** project 2 math
*/
#include "include/my.h"

void display_h(void)
{
    printf("USAGE\n");
    printf("\t./102architect x y transfo1 arg11 [arg12] [transfo2 arg21 [arg22]] ...\n");
    printf("DESCRIPTION\n");
    printf("\tx\tabscissa of the original point\n");
    printf("\ty\tordinate of the original point\n\n");
    printf("\ttansfo arg1 [arg2]\n");
    printf("\t-t i j translation along vector (i, j)\n");
    printf("\t-z m n scaling by factors m (x-axis) and n (y-axis)\n");
    printf("\t-r d   rotation centered in 0 by a d degree angle\n");
    printf("\t-s d   reflection over the axis passing through 0 with an inclination\n");
    printf("\t       angle of degrees\n");
}

void display_result(float *coord, float matrix [3][3])
{
    float transformed_coord[3] = {
        matrix[0][0] * coord[0] + matrix[0][1] * coord[1] + matrix[0][2],
        matrix[1][0] * coord[0] + matrix[1][1] * coord[1] + matrix[1][2],
        matrix[2][0] * coord[0] + matrix[2][1] * coord[1] + matrix[2][2]
    };
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%.2f\t", matrix[i][j]);
        }
        printf("\n");
    }
    printf("(%.2f, %.2f) => ", coord[0], coord[1]);
    printf("(%.2f, %.2f)\n", transformed_coord[0], transformed_coord[1]);
}

void xMatrix(Point *point, float matrix[3][3])
{
    float result[3] = {0};

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            result[i] += matrix[i][j] * point[j].x;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        point[i].x = result[i];
    }
}

float degreesToRadians(float degrees)
{
    return degrees * M_PI / 180.0;
}

TransformationMatrix translate(Point *point, Translation trans)
{
    float transMatrix[3][3] = {
        {1, 0, trans.tx},
        {0, 1, trans.ty},
        {0, 0, 1}};
    
    xMatrix(point, transMatrix);

    TransformationMatrix resultMatrix;
    memcpy(resultMatrix.matrix, transMatrix, sizeof(transMatrix));
    return resultMatrix;
}

TransformationMatrix scale(Point *point, Scaling scale)
{
    float scaleMatrix[3][3] = {
        {scale.sx, 0, 0},
        {0, scale.sy, 0},
        {0, 0, 1}};

    xMatrix(point, scaleMatrix);

    TransformationMatrix resultMatrix;
    memcpy(resultMatrix.matrix, scaleMatrix, sizeof(scaleMatrix));
    return resultMatrix;
}

TransformationMatrix rotate(Point *point, Rotation ruota)
{
    float angleInRadians = degreesToRadians(ruota.angleInDegrees);
    float costheta = cos(angleInRadians);
    float sintheta = sin(angleInRadians);
    float rotateMatrix[3][3] = {
        {costheta, -sintheta, 0},
        {sintheta, costheta, 0},
        {0, 0, 1}};

    xMatrix(point, rotateMatrix);

    TransformationMatrix resultMatrix;
    memcpy(resultMatrix.matrix, rotateMatrix, sizeof(rotateMatrix));
    return resultMatrix;
}

TransformationMatrix reflect(Point *point, Reflection rifletti)
{
    float angleInRadians = degreesToRadians(rifletti.angleInDegrees02);
    float costheta02 = cos(2 * angleInRadians);
    float sintheta02 = sin(2 * angleInRadians);
    float reflectMatrix[3][3] = {
        {costheta02, sintheta02, 0},
        {sintheta02, -costheta02, 0},
        {0, 0, 1}};

    xMatrix(point, reflectMatrix);

    TransformationMatrix resultMatrix;
    memcpy(resultMatrix.matrix, reflectMatrix, sizeof(reflectMatrix));
    return resultMatrix;
}
