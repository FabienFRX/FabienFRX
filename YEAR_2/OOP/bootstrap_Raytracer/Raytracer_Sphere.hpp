/*
** EPITECH PROJECT, 2024
** bootstrap_Raytracer
** File description:
** Sphere.hpp
*/

#ifndef RAYTRACER_SPHERE_HPP
#define RAYTRACER_SPHERE_HPP

#include "Raytracer_Ray.hpp" // Include our Ray class
#include <cmath>

namespace RayTracer {

class Sphere {
private:
    Math::Point3D center;
    double radius;

public:
    // Constructor that initializes the center and radius
    Sphere(const Math::Point3D& center, double radius)
        : center(center), radius(radius) {}

    // Hits method to determine if a ray intersects with the sphere
    bool hits(const Ray& ray) const {
        // Vector from ray origin to sphere center
        Math::Vector3D oc = ray.origin - center;
        
        // Coefficients for quadratic equation
        double a = ray.direction.dot(ray.direction); // Equivalent to direction.length()^2
        double b = 2.0 * oc.dot(ray.direction);
        double c = oc.dot(oc) - radius * radius;
        
        // Calculate discriminant
        double discriminant = b * b - 4 * a * c;
        
        // If discriminant is negative, there is no intersection
        // If discriminant is zero, ray touches the sphere at one point
        // If discriminant is positive, ray intersects the sphere at two points
        return discriminant >= 0;
    }
};

} // namespace RayTracer

#endif // RAYTRACER_SPHERE_H
