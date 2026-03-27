#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Engine.h"
#include "../Engine/Components/ShapeComponent.h"
#include "../Engine/Input/Input.h"
#include <cmath>

/**
 * @brief Obiekt statku gracza reprezentowany jako trojkat.
 * @ingroup module_demo
 */
class Ship : public GameObject {
private:
    const float ROTATION_SPEED = 0.1f; /**< Predkosc obrotu statku [rad/klatke]. */
    float thrust = 0.15f;              /**< Sila ciagu (zarezerwowana pod rozwoj fizyki). */
    float friction = 0.98f;            /**< Tlumienie ruchu (zarezerwowane pod rozwoj fizyki). */

public:
    /**
     * @brief Tworzy statek w zadanej pozycji.
     * @param startX Pozycja poczatkowa X.
     * @param startY Pozycja poczatkowa Y.
     */
    Ship(float startX, float startY) : GameObject("Player", startX, startY) {
        tag = "Player";

        // Wierzcholki trojkata sa zdefiniowane lokalnie wzgledem (0,0) obiektu.
        Triangle triangle = Triangle({0.0f, -20.0f}, {-15.0f, 15.0f}, {15.0f, 15.0f});

        AddComponent(std::make_unique<ShapeComponent>(triangle, Color::Red, true));
        AddComponent(std::make_unique<ShapeComponent>(triangle, Color::Black, false));
    }

    /**
     * @brief Aktualizuje sterowanie statkiem.
     */
    void Update() override {
        if (Input::IsActionHeld("MoveRight")) angle += ROTATION_SPEED;
        if (Input::IsActionHeld("MoveLeft")) angle -= ROTATION_SPEED;

        GameObject::Update();
    }
};
