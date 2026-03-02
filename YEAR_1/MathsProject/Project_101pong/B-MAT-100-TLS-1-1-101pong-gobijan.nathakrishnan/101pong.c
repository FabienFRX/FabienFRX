/*
** EPITECH PROJECT, 2023
** 101pong
** File description:
** ex.1 math
*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define M_PI 3.14159265358979323846

int check_num(char *param)
{
    for (int i = 0; param[i] != '\0'; i++) {
        if (param[i] < '0' && param[i] > '9') {
            return (84);
        }
    }
    return (0);
}

int main(int argc, char **argv)
{
    double xv;
    double yv;
    double zv;
    unsigned int n;
    double xvn;
    double yvn;
    double zvn;
    double distance;
    double incidence_angle;
    double incidence_angle_in_rad;

    if (argc <= 1) {
        printf("Error: no arguments");
        return (84);
    }
    if (argc > 1 && argc < 8) {
        printf("Error: too few arguments");
        return (84);
    }
    if (argc > 8) {
        printf("Error: too many arguments");
        return (84);
    }
    n = atoi(argv[7]);
    xv = atof(argv[4]) - atof(argv[1]);
    yv = atof(argv[5]) - atof(argv[2]);
    zv = atof(argv[6]) - atof(argv[3]);

    xvn = (xv * n) + atof(argv[4]);
    yvn = (yv * n) + atof(argv[5]);
    zvn = (zv * n) + atof(argv[6]);
    
    distance = sqrt(xv * xv + yv * yv + zv * zv);
    incidence_angle_in_rad = asin(zv / distance);
    incidence_angle = incidence_angle_in_rad * 180 / M_PI;
    incidence_angle *= -1;

    printf("The velocity vector of the ball is:\n");
    printf("(%.2f, %.2f, %.2f)\n", xv, yv, zv);
    printf("At time t + %u, ball coordinates will be:\n", n);
    printf("(%.2f, %.2f, %.2f)\n", xvn, yvn, zvn);

    if (atof(argv[6]) < 0)
    {
        printf("The ball won't reach the paddle.\n");
    }
    else
    {
        printf("The incidence angle is:\n");
        printf("%.2f degrees\n", incidence_angle);
    }
    return 0;
}
