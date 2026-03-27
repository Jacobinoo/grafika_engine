#include "Point2D.h"
#include <cmath>

/**
 * @brief Tworzy punkt/wektor 2D.
 */
Point2D::Point2D(float x, float y) : x(x), y(y) {}

/**
 * @brief Dodawanie dwoch wektorow.
 */
Point2D Point2D::operator+(const Point2D& other) const {
    return Point2D(x + other.x, y + other.y);
}

/**
 * @brief Odejmowanie dwoch wektorow.
 */
Point2D Point2D::operator-(const Point2D& other) const {
    return Point2D(x - other.x, y - other.y);
}

/**
 * @brief Mnozenie wektora przez skalar.
 */
Point2D Point2D::operator*(float scalar) const {
    return Point2D(x * scalar, y * scalar);
}

/**
 * @brief Obraca punkt wokol (0,0) o podany kat.
 */
Point2D Point2D::Rotate(float angleRadians) const {
    float cosA = std::cos(angleRadians);
    float sinA = std::sin(angleRadians);
    
    // Macierz rotacji 2D:
    // [x']   [ cos(a) -sin(a) ] [x]
    // [y'] = [ sin(a)  cos(a) ] [y]
    return Point2D(
        x * cosA - y * sinA,
        x * sinA + y * cosA
    );
}