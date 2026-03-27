#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Math/Point2D.h"
#include "Renderer/Renderer.h"
#include "Components/BaseComponent.h"

/**
 * @brief Bazowa jednostka sceny przechowujaca transformacje, komponenty i dzieci.
 *
 * Klasa implementuje prosty scene graph:
 * - pozycja/kat sa lokalne wzgledem rodzica,
 * - pozycja/kat w swiecie sa liczone rekurencyjnie.
 * @ingroup module_engine
 */
class GameObject {
public:
    /** @brief Etykieta obiektu uzywana np. do filtrowania lub kolizji. */
    std::string tag;

    /** @brief Flaga aktywnosci; gdy false, obiekt zostanie usuniety przy sprzataniu. */
    bool isActive;

    /** @brief Lokalna pozycja obiektu wzgledem rodzica. */
    Point2D position;

    /** @brief Lokalny kat obrotu obiektu (w radianach). */
    float angle;

    /** @brief Wskaznik na rodzica w drzewie sceny; nullptr oznacza korzen. */
    GameObject* parent;

    /** @brief Dzieci obiektu w scenie, przechowywane z semantyka wlacznej wlasnosci. */
    std::vector<std::unique_ptr<GameObject>> children;

    /** @brief Lista komponentow rozszerzajacych zachowanie obiektu. */
    std::vector<std::unique_ptr<BaseComponent>> components;

    /**
     * @brief Tworzy obiekt gry z podstawowa transformacja.
     * @param tag Nazwa identyfikacyjna obiektu.
     * @param x Poczatkowa wspolrzedna X.
     * @param y Poczatkowa wspolrzedna Y.
     */
    GameObject(const std::string& tag = "Default", float x = 0.0f, float y = 0.0f);

    /** @brief Wirtualny destruktor klasy bazowej. */
    virtual ~GameObject() = default;

    /**
     * @brief Aktualizuje komponenty i dzieci obiektu.
     */
    virtual void Update();

    /**
     * @brief Renderuje komponenty i dzieci obiektu.
     * @param renderer Referencja do renderera.
     */
    virtual void Render(Renderer& renderer);

    /**
     * @brief Dodaje dziecko do aktualnego obiektu i ustawia parent.
     * @param child Dziecko przekazywane przez unique_ptr.
     */
    void AddChild(std::unique_ptr<GameObject> child);

    /**
     * @brief Dodaje komponent i uruchamia jego inicjalizacje.
     * @param comp Komponent przekazywany przez unique_ptr.
     */
    void AddComponent(std::unique_ptr<BaseComponent> comp);

    /**
     * @brief Zwraca pierwszy komponent zadanego typu.
     * @tparam T Typ komponentu, np. ShapeComponent.
     * @return Wskaznik na komponent lub nullptr, gdy nie znaleziono.
     */
    template <typename T>
    T* GetComponent() {
        for (auto& comp : components) {
            // dynamic_cast zapewnia bezpieczne rzutowanie polimorficzne.
            T* target = dynamic_cast<T*>(comp.get());
            if (target) {
                return target;
            }
        }
        return nullptr;
    }

    /**
     * @brief Wyznacza pozycje obiektu w przestrzeni swiata.
     * @return Pozycja globalna wyliczona rekurencyjnie po drzewie rodzicow.
     */
    Point2D GetWorldPosition() const;

    /**
     * @brief Wyznacza kat obrotu obiektu w przestrzeni swiata.
     * @return Kat globalny liczony jako suma katow przodkow i lokalnego kata.
     */
    float GetWorldAngle() const;
};
