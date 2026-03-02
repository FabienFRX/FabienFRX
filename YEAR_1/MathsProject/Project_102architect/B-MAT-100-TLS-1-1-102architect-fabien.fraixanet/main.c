/*
** EPITECH PROJECT, 2023
** 102Architect
** File description:
** Math second project with Gobijan & Fabien
*/
#include "include/my.h"

int main(int argc, char *argv[])
{
    //if (argc < 6) {
    //    return 84;
    //}
    //return 0;
    //if (argc < 6) {
    //    return 84;
    //}
    //if (argc > 4 && (strcmp(argv[3], "-r") || strcmp(argv[3], "-s"))) {
    //    if (argc < 6) {
    //        printf("error\n");
    //        return 84;
    //    }
    //    return 0;
    //}
    
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        display_h();
        return 0;
    }
    if (argc < 4 || strlen(argv[3]) != 2) {
        return 84;
    }
    switch(argv[3][1]) {
        case 't':
        case 'z':
            if (argc !=6) {
                return (84);
            }
        break;
        case 'r':
        case 's':
            if (argc != 5) {
                return (84);
            }
        break;
        default:
            return (84);
    }
     
    Point p = {atof(argv[1]), atof(argv[2]), 1.0};
    float initial_coord[2] = {p.x, p.y};
    TransformationMatrix resultMatrix;

    for (int i = 3; i < argc; i += 2)
    {
        char Type = argv[i][1];
        switch (Type)
        {
        case 't':
            Translation transvalues = {.tx = atof(argv[i + 1]), .ty = atof(argv[i + 2])};
            resultMatrix = translate(&p, transvalues);
            printf("Translation along vector (%s, %s)\n", argv[i + 1], argv[i + 2]);
            display_result(initial_coord, resultMatrix.matrix);
            break;
        case 'z':
            Scaling scalevalues = {.sx = atof(argv[i + 1]), .sy = atof(argv[i + 2])};
            resultMatrix = scale(&p, scalevalues);
            printf("Scaling by factors %s and %s\n", argv[i + 1], argv[i + 2]);
            display_result(initial_coord, resultMatrix.matrix);
            break;
        case 'r':
            Rotation Rotatevalues = {.angleInDegrees = atof(argv[i + 1])};
            resultMatrix = rotate(&p, Rotatevalues);
            printf("Rotation by a %s degree angle\n", argv[i + 1]);
            display_result(initial_coord, resultMatrix.matrix);
            break;
        case 's':
            Reflection reflectvalues = {.angleInDegrees02 = atof(argv[i + 1])};
            resultMatrix = reflect(&p, reflectvalues);
            printf("Reflection over an axis with an inclination angle of %s degrees\n", argv[i + 1]);
            display_result(initial_coord, resultMatrix.matrix);
            break;
        }
    }
    return 0;
}