#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Engine.h"
#include "../Engine/Components/ShapeComponent.h"
#include "../Engine/Components/SpriteComponent.h"
#include "../Engine/Input/Input.h"
#include <cmath>

/**
 * @brief Obiekt pudelka reprezentowany przez prostokat.
 * @ingroup module_demo
 */
class Box : public GameObject {
private:
    const float ROTATION_SPEED = 0.1f; /**< Predkosc obrotu [rad/klatke]. */
    float thrust = 0.15f;              /**< Sila ciagu (zarezerwowana pod rozwoj). */
    float friction = 0.98f;            /**< Tlumienie ruchu (zarezerwowane pod rozwoj). */

public:
    /**
     * @brief Tworzy pudelko w zadanej pozycji.
     * @param startX Pozycja poczatkowa X.
     * @param startY Pozycja poczatkowa Y.
     */
    Box(float startX, float startY) : GameObject("Player", startX, startY) {
        tag = "Player";

        AddComponent(std::make_unique<SpriteComponent>(
            AssetManager::GetImage("Grass"), 0, 0, 16, 16
        ));
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
