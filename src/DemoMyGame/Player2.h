#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Engine.h"
#include "../Engine/Components/ShapeComponent.h"
#include "../Engine/Components/AnimatedSpriteComponent.h"
#include "../Engine/AssetManager.h"
#include "../Engine/Input/Input.h"
#include <cmath>

/**
 * @brief Obiekt gracza reprezentowany przez animowany sprite.
 * @ingroup module_demo
 */
class Player2 : public GameObject {
private:
    const float ROTATION_SPEED = 0.1f; /**< Predkosc obrotu statku [rad/klatke]. */
    float moveSpeed = 4.0f; /**< Predkosc chodzenia */

    AnimatedSpriteComponent* anim; /**< Wskaznik na komponent animacji, aby latwo zmieniac animacje w logice gry. */

public:
    /**
     * @brief Tworzy gracza w zadanej pozycji.
     * @param startX Pozycja poczatkowa X.
     * @param startY Pozycja poczatkowa Y.
     */
    Player2(float startX, float startY) : GameObject("Player2", startX, startY) {
        tag = "Player2";
        renderLayer = 100;

        scale = scale * 4.0f;

        // Wierzcholki trojkata sa zdefiniowane lokalnie wzgledem (0,0) obiektu.
        // Triangle triangle = Triangle({0.0f, -20.0f}, {-15.0f, 15.0f}, {15.0f, 15.0f});


        // 1. Tworzymy komponent podając tylko rozmiar klatki
        auto animPtr = std::make_unique<AnimatedSpriteComponent>(
            AssetManager::GetImage("Player2"), 16, 16
        );
        anim = animPtr.get();

        // Argumenty funkcji: Nazwa, StartCol, StartRow, Klatki, Szybkość, Zapętlanie
        anim->AddAnimation("WalkDown",  0, 0, 4, 6, true);
        anim->AddAnimation("WalkUp",    0, 1, 4, 6, true);
        anim->AddAnimation("WalkRight", 0, 2, 4, 6, true);
        anim->AddAnimation("WalkLeft",  0, 3, 4, 6, true);
        AddComponent(std::move(animPtr));
        
        

        anim->Play("WalkUp");
        anim->Stop();


        // AddComponent(std::make_unique<ShapeComponent>(triangle, Color::Red, true));
        // AddComponent(std::make_unique<ShapeComponent>(triangle, Color::Black, false));
    }

    /**
     * @brief Aktualizuje sterowanie graczem.
     */
    void Update() override {

        Point2D velocity(0.0f, 0.0f);

        if (Input::IsActionHeld("Player2MoveUp"))    velocity.y = -moveSpeed;
        if (Input::IsActionHeld("Player2MoveDown"))  velocity.y = moveSpeed;
        if (Input::IsActionHeld("Player2MoveLeft"))  velocity.x = -moveSpeed;
        if (Input::IsActionHeld("Player2MoveRight")) velocity.x = moveSpeed;

        if (velocity.x != 0 && velocity.y != 0) {
            velocity = velocity * 0.707f; // Normalizacja predkosci przy ruchu po skosie (1/sqrt(2))
            //Czemu 0.707f?
            //Poniewaz bez tego, poruszajac sie po skosie, poruszamy sie szybciej (ok. 1.41 raza) niz w poziomie/pionie.
            //Mnożąc przez 0.707f (czyli 1/sqrt(2)) normalizujemy tę prędkość, aby była spójna we wszystkich kierunkach.
        }

        position.x += velocity.x;
        position.y += velocity.y;

 
        if (velocity.y > 0)      anim->Play("WalkDown");
        else if (velocity.y < 0) anim->Play("WalkUp");
        else if (velocity.x < 0) anim->Play("WalkLeft");
        else if (velocity.x > 0) anim->Play("WalkRight");
        else                     anim->Stop();

        GameObject::Update();
    }
};
