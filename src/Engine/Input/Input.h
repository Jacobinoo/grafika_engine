#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>
#include "../Math/Point2D.h"
#include <unordered_map>
#include <string>

/** @brief Alias kodu klawisza SFML. */
using KeyCode = sf::Keyboard::Key;
/** @brief Alias kodu przycisku myszy SFML. */
using MouseCode = sf::Mouse::Button;

/**
 * @brief Statyczny modul obslugi wejscia (klawiatura + mysz).
 *
 * Modul bazuje na mapowaniu nazw akcji na fizyczne klawisze/przyciski.
 * Dzieki temu kod gry operuje na akcjach logicznych (np. "Jump").
 * @ingroup module_input
 */
class Input {
private:
    static std::unordered_map<std::string, KeyCode> actionBindings;      /**< Powiazanie nazwy akcji z klawiszem. */
    static std::unordered_map<std::string, MouseCode> mouseActionBindings; /**< Powiazanie nazwy akcji z przyciskiem myszy. */

    static std::unordered_map<KeyCode, bool> currentKeyStates;    /**< Aktualny stan klawiszy. */
    static std::unordered_map<KeyCode, bool> previousKeyStates;   /**< Stan klawiszy z poprzedniej klatki. */
    
    static std::unordered_map<MouseCode, bool> currentMouseStates;  /**< Aktualny stan przyciskow myszy. */
    static std::unordered_map<MouseCode, bool> previousMouseStates; /**< Stan przyciskow myszy z poprzedniej klatki. */

public:
    /**
     * @brief Rejestruje akcje klawiaturowa.
     * @param actionName Nazwa akcji logicznej.
     * @param key Przypisany klawisz.
     */
    static void BindAction(const std::string& actionName, KeyCode key);

    /**
     * @brief Rejestruje akcje myszy.
     * @param actionName Nazwa akcji logicznej.
     * @param button Przypisany przycisk myszy.
     */
    static void BindMouseAction(const std::string& actionName, MouseCode button);

    /**
     * @brief Aktualizuje stany wejscia.
     *
     * Powinna byc wywolywana raz na klatke przed logika gry.
     */
    static void Update();

    /**
     * @brief Sprawdza czy akcja jest aktualnie przytrzymywana.
     * @param actionName Nazwa akcji.
     * @return true gdy przycisk akcji jest wcisniety.
     */
    static bool IsActionHeld(const std::string& actionName);

    /**
     * @brief Sprawdza zbocze narastajace (fresh press) akcji.
     * @param actionName Nazwa akcji.
     * @return true tylko w klatce przejscia false -> true.
     */
    static bool IsActionJustPressed(const std::string& actionName);

    /**
     * @brief Zwraca pozycje kursora myszy wzgledem danego okna.
     * @param window Okno referencyjne.
     * @return Pozycja kursora jako @ref Point2D.
     */
    static Point2D GetMousePosition(const sf::RenderWindow& window);
};