#include "Input.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <unordered_map>
#include <string>

// Inicjalizacja statycznych zmiennych
std::unordered_map<std::string, KeyCode> Input::actionBindings;
std::unordered_map<KeyCode, bool> Input::currentKeyStates;
std::unordered_map<KeyCode, bool> Input::previousKeyStates;

std::unordered_map<std::string, MouseCode> Input::mouseActionBindings;
std::unordered_map<MouseCode, bool> Input::currentMouseStates;
std::unordered_map<MouseCode, bool> Input::previousMouseStates;

void Input::BindAction(const std::string& actionName, KeyCode key) {
    actionBindings[actionName] = key;
    currentKeyStates[key] = false;
    previousKeyStates[key] = false;
}

void Input::BindMouseAction(const std::string& actionName, MouseCode button) {
    mouseActionBindings[actionName] = button;
    currentMouseStates[button] = false;
    previousMouseStates[button] = false;
}

void Input::Update() {
    //KLAWIATURA
    // Kopiujemy obecny stan do "poprzedniego"
    previousKeyStates = currentKeyStates;
    for (auto& pair : currentKeyStates) {
        pair.second = sf::Keyboard::isKeyPressed(pair.first);
    }

    previousMouseStates = currentMouseStates;
    for (auto& pair : currentMouseStates) {
        pair.second = sf::Mouse::isButtonPressed(pair.first);
    }
}

bool Input::IsActionJustPressed(const std::string& actionName) {
    // Czy to akcja przypisana do klawiatury?
    auto itKey = actionBindings.find(actionName);
    if (itKey != actionBindings.end()) {
        KeyCode key = itKey->second;
        return currentKeyStates[key] && !previousKeyStates[key];
    }

    // Czy to akcja przypisana do myszki?
    auto itMouse = mouseActionBindings.find(actionName);
    if (itMouse != mouseActionBindings.end()) {
        sf::Mouse::Button btn = itMouse->second;
        return currentMouseStates[btn] && !previousMouseStates[btn];
    }

    return false;
}

bool Input::IsActionHeld(const std::string& actionName) {
    auto itKey = actionBindings.find(actionName);
        if (itKey != actionBindings.end()) return currentKeyStates[itKey->second];

        auto itMouse = mouseActionBindings.find(actionName);
        if (itMouse != mouseActionBindings.end()) return currentMouseStates[itMouse->second];

        return false;
}
