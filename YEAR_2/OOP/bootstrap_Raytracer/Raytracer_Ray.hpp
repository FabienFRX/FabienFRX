/*
** EPITECH PROJECT, 2024
** bootstrap_Raytracer
** File description:
** Ray.hpp
*/

#ifndef RAYTRACER_RAY_HPP
#define RAYTRACER_RAY_HPP

#include "Math_Vector3D.hpp" // Include the Vector3D class

namespace RayTracer {

class Ray {
public:
    Math::Point3D origin;
    Math::Vector3D direction;

    // Constructeur par défaut avec des attributs construits par défaut
    Ray() = default;

    // Constructeur prenant un Point3D et un Vector3D en paramètres
    Ray(const Math::Point3D& origin, const Math::Vector3D& direction)
        : origin(origin), direction(direction) {}
};

} // namespace RayTracer

#endif // RAYTRACER_RAY_HPP