#pragma once
#include "Point2D.h"
#include <vector>

/**
 * @brief Odcinek zdefiniowany przez dwa punkty koncowe.
 * @ingroup module_math
 */
class LineSegment {
public:
    Point2D p1; /**< Punkt poczatkowy. */
    Point2D p2; /**< Punkt koncowy. */

    /**
     * @brief Tworzy odcinek.
     * @param p1 Punkt poczatkowy.
     * @param p2 Punkt koncowy.
     */
    LineSegment(Point2D p1, Point2D p2) : p1(p1), p2(p2) {}
};

/**
 * @brief Trojkat zdefiniowany przez trzy wierzcholki.
 * @ingroup module_math
 */
class Triangle {
public:
    Point2D p1; /**< Pierwszy wierzcholek. */
    Point2D p2; /**< Drugi wierzcholek. */
    Point2D p3; /**< Trzeci wierzcholek. */

    /**
     * @brief Tworzy trojkat.
     * @param p1 Pierwszy wierzcholek.
     * @param p2 Drugi wierzcholek.
     * @param p3 Trzeci wierzcholek.
     */
    Triangle(Point2D p1, Point2D p2, Point2D p3) : p1(p1), p2(p2), p3(p3) {}
};

/**
 * @brief Prostokat osiowo wyrownany (axis-aligned) w przestrzeni lokalnej.
 * @ingroup module_math
 */
class Rectangle {
public:
    Point2D topLeft; /**< Lewy gorny rog prostokata. */
    float width;     /**< Szerokosc prostokata. */
    float height;    /**< Wysokosc prostokata. */

    /**
     * @brief Tworzy prostokat.
     * @param topLeft Lewy gorny rog.
     * @param width Szerokosc.
     * @param height Wysokosc.
     */
    Rectangle(Point2D topLeft, float width, float height) 
        : topLeft(topLeft), width(width), height(height) {}
};

/**
 * @brief Kolo/okrag zdefiniowany przez srodek i promien.
 * @ingroup module_math
 */
class Circle {
public:
    Point2D center; /**< Srodek kola. */
    float radius;   /**< Promien kola. */

    /**
     * @brief Tworzy kolo.
     * @param center Srodek.
     * @param radius Promien.
     */
    Circle(Point2D center, float radius) : center(center), radius(radius) {}
};

class Ellipse {
public:
    Point2D center;
    float rx; // Promień poziomy
    float ry; // Promień pionowy
    
    Ellipse(Point2D center, float rx, float ry) 
        : center(center), rx(rx), ry(ry) {}
};

/**
 * @brief Wielokat opisany przez liste wierzcholkow.
 * @ingroup module_math
 */
class Polygon {
public:
    std::vector<Point2D> vertices; /**< Wierzcholki wielokata, podane w kolejnosci obiegu. */

    /**
     * @brief Tworzy wielokat.
     * @param vertices Lista wierzcholkow.
     */
    Polygon(const std::vector<Point2D>& vertices) : vertices(vertices) {}
};