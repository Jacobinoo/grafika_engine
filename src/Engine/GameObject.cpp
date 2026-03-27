#include "GameObject.h"
#include <algorithm>

/**
 * @brief Tworzy obiekt gry i ustawia wartosci domyslne.
 */
GameObject::GameObject(const std::string& tag, float x, float y)
    : tag(tag), position(x, y), angle(0.0f), isActive(true), parent(nullptr) {
}

/**
 * @brief Aktualizuje komponenty i poddrzewo dzieci.
 */
void GameObject::Update() {
    // 1) Komponenty obiektu.
    for (auto& comp : components) {
        comp->Update();
    }

    // 2) Dzieci w drzewie sceny.
    for (auto& child : children) {
        if (child->isActive) {
            child->Update();
        }
    }

    // 3) Sprzatanie nieaktywnych dzieci (remove-erase idiom).
    children.erase(
        std::remove_if(children.begin(), children.end(),
            [](const std::unique_ptr<GameObject>& child) {
                return !child->isActive;
            }),
        children.end()
    );
}

/**
 * @brief Renderuje komponenty i dzieci obiektu.
 */
void GameObject::Render(Renderer& renderer) {
    // 1) Render komponentow obiektu.
    for (auto& comp : components) {
        comp->Draw(renderer);
    }

    // 2) Rekurencyjny render dzieci.
    for (auto& child : children) {
        if (child->isActive) {
            child->Render(renderer);
        }
    }
}

/**
 * @brief Dodaje dziecko i podpina relacje parent-child.
 */
void GameObject::AddChild(std::unique_ptr<GameObject> child) {
    child->parent = this;
    children.push_back(std::move(child));
}

/**
 * @brief Dodaje komponent do obiektu.
 */
void GameObject::AddComponent(std::unique_ptr<BaseComponent> comp) {
    comp->gameObject = this;
    comp->Init();
    components.push_back(std::move(comp));
}

/**
 * @brief Zwraca pozycje obiektu w przestrzeni swiata.
 */
Point2D GameObject::GetWorldPosition() const {
    if (parent) {
        // Kluczowa transformacja hierarchiczna (scene graph):
        // p_world = p_parent_world + R(parent_angle) * p_local
        // gdzie R(theta) to macierz rotacji 2D.
        return parent->GetWorldPosition() + position.Rotate(parent->GetWorldAngle());
    }
    // Korzen drzewa: pozycja lokalna jest rowna globalnej.
    return position;
}

/**
 * @brief Zwraca kat obrotu obiektu w przestrzeni swiata.
 */
float GameObject::GetWorldAngle() const {
    if (parent) {
        // Dla transformacji 2D kat globalny to suma katow na sciezce rodzicow.
        return parent->GetWorldAngle() + angle;
    }
    return angle;
}
