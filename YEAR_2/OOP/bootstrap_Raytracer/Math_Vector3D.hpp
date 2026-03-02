/*
** EPITECH PROJECT, 2024
** bootstrap_Raytracer
** File description:
** Vector_Point.hpp
*/

#ifndef MATH_VECTOR3D_HPP
#define MATH_VECTOR3D_HPP

#include <cmath>

namespace Math {

class Vector3D {
public:
    double X, Y, Z;

    // Constructeur par défaut initialisant les composantes à 0
    Vector3D() = default;

    // Constructeur prenant 3 composantes en paramètres
    Vector3D(double x, double y, double z) : X(x), Y(y), Z(z) {}

    // Retourne la longueur du vecteur
    double length() const {
        return std::sqrt(X * X + Y * Y + Z * Z);
    }

    // Produit scalaire de deux vecteurs
    double dot(const Vector3D& other) const {
        return X * other.X + Y * other.Y + Z * other.Z;
    }

    // Opérateurs avec un autre Vector3D
    Vector3D operator+(const Vector3D& other) const {
        return Vector3D(X + other.X, Y + other.Y, Z + other.Z);
    }

    Vector3D& operator+=(const Vector3D& other) {
        X += other.X;
        Y += other.Y;
        Z += other.Z;
        return *this;
    }

    Vector3D operator-(const Vector3D& other) const {
        return Vector3D(X - other.X, Y - other.Y, Z - other.Z);
    }

    Vector3D& operator-=(const Vector3D& other) {
        X -= other.X;
        Y -= other.Y;
        Z -= other.Z;
        return *this;
    }

    Vector3D operator*(const Vector3D& other) const {
        return Vector3D(X * other.X, Y * other.Y, Z * other.Z);
    }

    Vector3D& operator*=(const Vector3D& other) {
        X *= other.X;
        Y *= other.Y;
        Z *= other.Z;
        return *this;
    }

    Vector3D operator/(const Vector3D& other) const {
        return Vector3D(X / other.X, Y / other.Y, Z / other.Z);
    }

    Vector3D& operator/=(const Vector3D& other) {
        X /= other.X;
        Y /= other.Y;
        Z /= other.Z;
        return *this;
    }

    // Opérateurs avec un scalaire (double)
    Vector3D operator*(double scalar) const {
        return Vector3D(X * scalar, Y * scalar, Z * scalar);
    }

    Vector3D& operator*=(double scalar) {
        X *= scalar;
        Y *= scalar;
        Z *= scalar;
        return *this;
    }

    Vector3D operator/(double scalar) const {
        return Vector3D(X / scalar, Y / scalar, Z / scalar);
    }

    Vector3D& operator/=(double scalar) {
        X /= scalar;
        Y /= scalar;
        Z /= scalar;
        return *this;
    }
    
    // Transformations géométriques
    Vector3D& translate(const Vector3D& translation) {
        X += translation.X;
        Y += translation.Y;
        Z += translation.Z;
        return *this;
    }
    
    // Rotation autour de l'axe X (angle en degrés)
    Vector3D& rotateX(double degrees) {
        double radians = degrees * M_PI / 180.0;
        double cosR = std::cos(radians);
        double sinR = std::sin(radians);
        
        double oldY = Y;
        double oldZ = Z;
        
        Y = oldY * cosR - oldZ * sinR;
        Z = oldY * sinR + oldZ * cosR;
        
        return *this;
    }
    
    // Rotation autour de l'axe Y (angle en degrés)
    Vector3D& rotateY(double degrees) {
        double radians = degrees * M_PI / 180.0;
        double cosR = std::cos(radians);
        double sinR = std::sin(radians);
        
        double oldX = X;
        double oldZ = Z;
        
        X = oldX * cosR + oldZ * sinR;
        Z = -oldX * sinR + oldZ * cosR;
        
        return *this;
    }
    
    // Rotation autour de l'axe Z (angle en degrés)
    Vector3D& rotateZ(double degrees) {
        double radians = degrees * M_PI / 180.0;
        double cosR = std::cos(radians);
        double sinR = std::sin(radians);
        
        double oldX = X;
        double oldY = Y;
        
        X = oldX * cosR - oldY * sinR;
        Y = oldX * sinR + oldY * cosR;
        
        return *this;
    }
};

// Opérateur ami pour la multiplication scalaire (permet scalar * vector)
inline Vector3D operator*(double scalar, const Vector3D& vector) {
    return vector * scalar;
}

class Point3D {
public:
    double X, Y, Z;

    // Constructeur par défaut initialisant les composantes à 0
    Point3D() = default;

    // Constructeur prenant 3 composantes en paramètres
    Point3D(double x, double y, double z) : X(x), Y(y), Z(z) {}

    // Opérations entre Point3D et Vector3D
    Point3D operator+(const Vector3D& vector) const {
        return Point3D(X + vector.X, Y + vector.Y, Z + vector.Z);
    }

    Point3D& operator+=(const Vector3D& vector) {
        X += vector.X;
        Y += vector.Y;
        Z += vector.Z;
        return *this;
    }

    Vector3D operator-(const Point3D& other) const {
        return Vector3D(X - other.X, Y - other.Y, Z - other.Z);
    }
};

} // namespace Math

#endif // MATH_VECTOR3D_HPP
