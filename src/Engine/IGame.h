#pragma once

/**
 * @brief Interfejs bazowy logiki gry uruchamianej przez silnik.
 *
 * Silnik wywoluje metody tego interfejsu w cyklu zycia aplikacji:
 * - @ref Start() raz po inicjalizacji,
 * - @ref Update() raz na klatke.
 * @ingroup module_engine
 */
class IGame {
public:
    /**
     * @brief Wirtualny destruktor wymagany do poprawnego usuwania po wskazniku bazowym.
     */
    virtual ~IGame() = default;

    /**
     * @brief Jednorazowa inicjalizacja zasobow gry.
     */
    virtual void Start() = 0;

    /**
     * @brief Aktualizacja logiki gry wykonywana w kazdej klatce.
     */
    virtual void Update() = 0;
};