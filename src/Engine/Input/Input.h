#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>
#include <unordered_map>
#include <string>

using KeyCode = sf::Keyboard::Key;
using MouseCode = sf::Mouse::Button;

class Input {
private:
    // Action Bindings
    static std::unordered_map<std::string, KeyCode> actionBindings;
    static std::unordered_map<std::string, MouseCode> mouseActionBindings;

    // Stany
    static std::unordered_map<KeyCode, bool> currentKeyStates;
    static std::unordered_map<KeyCode, bool> previousKeyStates;
    
    static std::unordered_map<MouseCode, bool> currentMouseStates;
    static std::unordered_map<MouseCode, bool> previousMouseStates;

public:
    // Konfiguracja klawiatury i myszy
    static void BindAction(const std::string& actionName, KeyCode key);
    static void BindMouseAction(const std::string& actionName, MouseCode button);

    // Pętla silnika
    static void Update();

    // API dla twórcy gier
    static bool IsActionHeld(const std::string& actionName);
    static bool IsActionJustPressed(const std::string& actionName);
};