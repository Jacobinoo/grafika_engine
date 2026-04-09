#pragma once

// Wczesna deklaracja zapobiega zapętleniu plików nagłówkowych
class GameObject;
class Renderer;

/**
 * @brief Bazowa klasa komponentu w architekturze ECS-like (kompozycja obiektu).
 * @ingroup module_components
 */
class BaseComponent {
public:
    /**
     * @brief Wskaznik na obiekt, do ktorego komponent jest przypiety.
     *
     * Pole ustawia @ref GameObject::AddComponent.
     */
    GameObject* gameObject = nullptr;

    /** @brief Wirtualny destruktor klasy bazowej. */
    virtual ~BaseComponent() = default;

    /** @brief Hook inicjalizacyjny wywolywany po podpieciu komponentu. */
    virtual void Init() {}

    /** @brief Hook aktualizacji wykonywany co klatke. */
    virtual void Update() {}

    /**

     * @brief Hook rysowania wykonywany podczas renderowania obiektu.
     * @param renderer Referencja do renderera.
     */
    virtual void Draw(Renderer& renderer) {}
};
