#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Engine.h"
#include "../Engine/Components/ShapeComponent.h"
#include "../Engine/Input/Input.h"
#include <cmath>

/**
 * @brief Obiekt wielokatu reprezentowany przez wielokat.
 * @ingroup module_demo
 */
class Poly : public GameObject {
private:
    const float ROTATION_SPEED = 0.1f; /**< Predkosc obrotu [rad/klatke]. */
    float thrust = 0.15f;              /**< Sila ciagu (zarezerwowana pod rozwoj). */
    float friction = 0.98f;            /**< Tlumienie ruchu (zarezerwowane pod rozwoj). */

public:
    /**
     * @brief Tworzy wielokat w zadanej pozycji.
     * @param startX Pozycja poczatkowa X.
     * @param startY Pozycja poczatkowa Y.
     */
    Poly(float startX, float startY) : GameObject("Player", startX, startY) {
        tag = "Player";

        // Wierzcholki nieregularnego "diamenciku" z lekko wydluzonym dziobem.
        std::vector<Point2D> verts = {
            {0.0f, -24.0f},
            {16.0f, -12.0f},
            {20.0f, 4.0f},
            {10.0f, 18.0f},
            {-10.0f, 18.0f},
            {-20.0f, 4.0f},
            {-16.0f, -12.0f}
        };


        // Wielokat jest zdefiniowany lokalnie wzgledem srodka obiektu.
        Polygon poly(verts);

        AddComponent(std::make_unique<ShapeComponent>(poly, Color(165, 42, 42, 255), true));
        AddComponent(std::make_unique<ShapeComponent>(poly, Color::Black, false));
    }

    /**
     * @brief Aktualizuje sterowanie obrotem pudelka.
     */
    void Update() override {
        if (Input::IsActionHeld("BoxMoveRight")) angle += ROTATION_SPEED;
        if (Input::IsActionHeld("BoxMoveLeft")) angle -= ROTATION_SPEED;

        GameObject::Update();
    }
};
