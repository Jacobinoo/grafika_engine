#pragma once

/**
 * @brief Prosta struktura wektora/punktu 2D z podstawowymi operacjami.
 * @ingroup module_math
 */
class Point2D {
public:
    float x; /**< Wspolrzedna pozioma. */
    float y; /**< Wspolrzedna pionowa. */

    /**
     * @brief Tworzy punkt 2D.
     * @param x Wspolrzedna X.
     * @param y Wspolrzedna Y.
     */
    Point2D(float x = 0.0f, float y = 0.0f);

    /**
     * @brief Dodawanie wektorow/punktow.
     * @param other Drugi skladnik.
     * @return Suma skladowych.
     */
    Point2D operator+(const Point2D& other) const;

    /**
     * @brief Odejmowanie wektorow/punktow.
     * @param other Odjemnik.
     * @return Roznica skladowych.
     */
    Point2D operator-(const Point2D& other) const;

    /**
     * @brief Skalowanie przez skalar.
     * @param scalar Mnoznik.
     * @return Wektor po skalowaniu.
     */
    Point2D operator*(float scalar) const;

    /**
     * @brief Obraca punkt wokol poczatku ukladu (0,0).
     * @param angleRadians Kat obrotu w radianach.
     * @return Punkt po obrocie.
     */
    Point2D Rotate(float angleRadians) const;
};