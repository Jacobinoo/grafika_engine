#include "Input.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <unordered_map>
#include <string>

/** @brief Mapa nazw akcji klawiatury do klawiszy SFML. */
std::unordered_map<std::string, KeyCode> Input::actionBindings;
/** @brief Aktualny stan klawiszy. */
std::unordered_map<KeyCode, bool> Input::currentKeyStates;
/** @brief Stan klawiszy z poprzedniej klatki. */
std::unordered_map<KeyCode, bool> Input::previousKeyStates;

/** @brief Mapa nazw akcji myszy do przyciskow SFML. */
std::unordered_map<std::string, MouseCode> Input::mouseActionBindings;
/** @brief Aktualny stan przyciskow myszy. */
std::unordered_map<MouseCode, bool> Input::currentMouseStates;
/** @brief Stan przyciskow myszy z poprzedniej klatki. */
std::unordered_map<MouseCode, bool> Input::previousMouseStates;

/**
 * @brief Przypisuje nazwe akcji do klawisza.
 */
void Input::BindAction(const std::string& actionName, KeyCode key) {
    actionBindings[actionName] = key;
    // Inicjalizacja stanow startowych usuwa ryzyko odczytu "smieci" logicznych.
    currentKeyStates[key] = false;
    previousKeyStates[key] = false;
}

/**
 * @brief Przypisuje nazwe akcji do przycisku myszy.
 */
void Input::BindMouseAction(const std::string& actionName, MouseCode button) {
    mouseActionBindings[actionName] = button;
    currentMouseStates[button] = false;
    previousMouseStates[button] = false;
}

/**
 * @brief Aktualizuje snapshot stanu wejscia dla biezacej klatki.
 */
void Input::Update() {
    // Logika edge-trigger:
    // - previous <- current
    // - current <- odczyt z urzadzenia
    // Dzieki temu mozna wykryc przejscie 0->1 (JustPressed).
    previousKeyStates = currentKeyStates;
    for (auto& pair : currentKeyStates) {
        pair.second = sf::Keyboard::isKeyPressed(pair.first);
    }

    previousMouseStates = currentMouseStates;
    for (auto& pair : currentMouseStates) {
        // Sprawdzamy, czy przycisk myszy jest aktualnie wciśnięty.
        pair.second = sf::Mouse::isButtonPressed(pair.first);
    }
}

/**
 * @brief Sprawdza czy akcja zostala nacisnieta w tej klatce.
 */
bool Input::IsActionJustPressed(const std::string& actionName) {
    auto itKey = actionBindings.find(actionName);
    if (itKey != actionBindings.end()) {
        KeyCode key = itKey->second;
        return currentKeyStates[key] && !previousKeyStates[key];
    }

    auto itMouse = mouseActionBindings.find(actionName);
    if (itMouse != mouseActionBindings.end()) {
        sf::Mouse::Button btn = itMouse->second;
        return currentMouseStates[btn] && !previousMouseStates[btn];
    }

    return false;
}

/**
 * @brief Sprawdza czy akcja jest przytrzymywana.
 */
bool Input::IsActionHeld(const std::string& actionName) {
    auto itKey = actionBindings.find(actionName);
    if (itKey != actionBindings.end()) {
        return currentKeyStates[itKey->second];
    }

    auto itMouse = mouseActionBindings.find(actionName);
    if (itMouse != mouseActionBindings.end()) {
        return currentMouseStates[itMouse->second];
    }

    return false;
}

/**
 * @brief Pobiera pozycje kursora myszy wzgledem wskazanego okna.
 */
Point2D Input::GetMousePosition(const sf::RenderWindow& window) {
    sf::Vector2i pos = sf::Mouse::getPosition(window);
    return Point2D((float)pos.x, (float)pos.y);
}