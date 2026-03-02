/*
** EPITECH PROJECT, 2024
** bootstrap_Raytracer
** File description:
** Raytracer_Rectangle.hpp
*/

#ifndef RAYTRACER_RECTANGLE_HPP
#define RAYTRACER_RECTANGLE_HPP

#include "Raytracer_Ray.hpp" // Inclusion de notre classe Ray

namespace RayTracer {

class Rectangle3D {
public:
    Math::Point3D origin;       // Coin inférieur gauche
    Math::Vector3D bottom_side; // Vecteur du coin inférieur gauche au coin inférieur droit
    Math::Vector3D left_side;   // Vecteur du coin inférieur gauche au coin supérieur gauche

    // Constructeur par défaut
    Rectangle3D() = default;

    // Constructeur
    Rectangle3D(const Math::Point3D& origin, 
               const Math::Vector3D& bottom_side, 
               const Math::Vector3D& left_side)
        : origin(origin), bottom_side(bottom_side), left_side(left_side) {}

    // Retourne les coordonnées 3D pour un point dans le rectangle à (u,v)
    // où u et v sont dans l'intervalle [0;1]
    Math::Point3D pointAt(double u, double v) const {
        // Calcule le point dans l'espace 3D basé sur les coordonnées u,v
        // origin + u*bottom_side + v*left_side
        return origin + (bottom_side * u) + (left_side * v);
    }
};

class Camera {
public:
    Math::Point3D origin; // Position de la caméra (par défaut à 0,0,0)
    Rectangle3D screen;   // Rectangle d'écran

    // Constructeur par défaut
    Camera() 
        : origin(Math::Point3D(0, 0, 0)) {
        // Écran par défaut : rectangle 2x2 centré à z=-1
        screen = Rectangle3D(
            Math::Point3D(-1, -1, -1),      // origine (coin inférieur gauche)
            Math::Vector3D(2, 0, 0),        // bottom_side: largeur 2 le long de l'axe x
            Math::Vector3D(0, 2, 0)         // left_side: hauteur 2 le long de l'axe y
        );
    }

    // Constructeur avec origine et écran personnalisés
    Camera(const Math::Point3D& origin, const Rectangle3D& screen)
        : origin(origin), screen(screen) {}

    // Constructeur de copie
    Camera(const Camera& other) = default;

    // Opérateur d'affectation de copie
    Camera& operator=(const Camera& other) = default;

    // Destructeur
    ~Camera() = default;

    // Crée un rayon de l'origine de la caméra au point à (u,v) sur l'écran
    Ray ray(double u, double v) const {
        // Obtient le point 3D sur l'écran
        Math::Point3D point = screen.pointAt(u, v);
        
        // Crée un vecteur de direction de la caméra au point
        Math::Vector3D direction = point - origin;
        
        // Retourne un rayon de l'origine de la caméra vers le point
        return Ray(origin, direction);
    }
};

} // namespace RayTracer

#endif // RAYTRACER_RECTANGLE_HPP
