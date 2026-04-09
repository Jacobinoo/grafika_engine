#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Engine.h"
#include "../Engine/Components/ShapeComponent.h"
#include "../Engine/Input/Input.h"

/**
 * @brief Obiekt UFO reprezentowany jako okrag z reakcja na klikniecie myszy.
 * @ingroup module_demo
 */
class Ufo : public GameObject {
private:
    const float ROTATION_SPEED = 0.1f; /**< Predkosc obrotu UFO [rad/klatke]. */
    float thrust = 0.15f;              /**< Sila ciagu (zarezerwowana pod rozwoj). */
    float friction = 0.98f;            /**< Tlumienie ruchu (zarezerwowane pod rozwoj). */

public:
    /**
     * @brief Tworzy UFO w zadanej pozycji.
     * @param startX Pozycja poczatkowa X.
     * @param startY Pozycja poczatkowa Y.
     */
    Ufo(float startX, float startY) : GameObject("Player", startX, startY) {
        tag = "Player";

        // Elipsa
        Ellipse ellipse = Ellipse({0.0f, 0.0f}, 20.0f, 10.0f);

        AddComponent(std::make_unique<ShapeComponent>(ellipse, Color::Magenta, true));


        /** @brief Szerokosc ekranu (obecnie nieuzywana). */
        unsigned int width = Engine::GetInstance().GetScreenWidth();
        /** @brief Wysokosc ekranu (obecnie nieuzywana). */
        unsigned int height = Engine::GetInstance().GetScreenHeight();
    }

    /**
     * @brief Aktualizuje sterowanie i logike interakcji kursora z UFO.
     */
    void Update() override {
        if (Input::IsActionHeld("UfoMoveRight")) angle += ROTATION_SPEED;
        if (Input::IsActionHeld("UfoMoveLeft")) angle -= ROTATION_SPEED;

        auto shapeComp = GetComponent<ShapeComponent>();

        if(shapeComp) {
            Engine& engine = Engine::GetInstance();

            Point2D mousePos = Input::GetMousePosition(engine.GetWindow());
            Point2D pos = GetWorldPosition();

            Point2D d = mousePos - pos;

            bool isHovered = ((d.x * d.x) + (d.y * d.y)) <= (20.0f * 20.0f);

            if (isHovered && Input::IsActionJustPressed("Click")) {
                shapeComp->SetColor(Color::Yellow);

                ShapeType newEllipse = shapeComp->GetShape();
                if (std::holds_alternative<Ellipse>(newEllipse)) {
                    Ellipse e = std::get<Ellipse>(newEllipse);
                    e.rx += 50.0f; // Powiększamy promień poziomy
                    e.ry += 50.0f; //promien pionowy
                    shapeComp->SetShape(e);
                }
            }
        }
        GameObject::Update();
    }
};
