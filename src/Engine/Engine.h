#pragma once
#include "GameObject.h"
#include "IGame.h"
#include "Renderer/Renderer.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <memory>
#include <string>

using Color = sf::Color;

/**
 * @brief Dostepne tryby tworzenia okna aplikacji.
 * @ingroup module_engine
 */
enum class WindowMode {
    WINDOWED,     /**< Klasyczne okno systemowe z ramka i paskiem tytulu. */
    FULLSCREEN,   /**< Pelny ekran w trybie ekskluzywnym monitora. */
    BORDERLESS    /**< Okno bez ramki, zwykle wykorzystywane jako pseudo-fullscreen. */
};

/**
 * @brief Konfiguracja startowa silnika.
 * @ingroup module_engine
 */
struct EngineConfig {
    int width = 1280;                         /**< Szerokosc okna w pikselach. */
    int height = 720;                         /**< Wysokosc okna w pikselach. */
    std::string title = "Engine Window";      /**< Tytul wyswietlany na pasku okna. */
    WindowMode windowMode = WindowMode::WINDOWED; /**< Tryb prezentacji okna. */
    int framerateLimit = 60;                  /**< Docelowy limit FPS; 0 oznacza fallback do 60. */
};

/**
 * @brief Glowna klasa silnika zarzadzajaca petla gry, oknem i obiektami sceny.
 *
 * Klasa jest zaimplementowana jako singleton. Uzywaj @ref GetInstance().
 * @ingroup module_engine
 */
class Engine {
private:
    std::unique_ptr<sf::RenderWindow> window; /**< Okno SFML odpowiedzialne za prezentacje obrazu. */
    std::unique_ptr<Renderer> renderer;       /**< Wlasny renderer programowy zapisujacy piksele do bufora. */
    unsigned int targetFramerate = 60;        /**< Aktualnie ustawiony limit klatek na sekunde. */

    std::vector<std::unique_ptr<GameObject>> gameObjects; /**< Aktywne obiekty obecne w scenie. */
    std::vector<std::unique_ptr<GameObject>> objectQueue; /**< Kolejka obiektow dodawanych bezpiecznie po aktualizacji. */

    sf::Color currentBackgroundColor = sf::Color::Black; /**< Kolor tla czyszczenia klatki. */

    bool isRunning = false; /**< Flaga sterujaca petla glowna silnika. */

    /** @brief Prywatny konstruktor wymuszajacy singleton. */
    Engine() = default;
    /** @brief Prywatny destruktor singletonu. */
    ~Engine() = default;

    /** @brief Kopiowanie singletonu jest zabronione. */
    Engine(const Engine&) = delete;
    /** @brief Przypisanie singletonu jest zabronione. */
    Engine& operator=(const Engine&) = delete;

public:
    /**
     * @brief Zwraca globalna instancje silnika.
     * @return Referencja do singletonu @ref Engine.
     */
    static Engine& GetInstance() {
        static Engine instance;
        return instance;
    }

    /**
     * @brief Inicjalizuje okno, renderer oraz podstawowe zasoby silnika.
     * @param config Struktura konfiguracji uruchomienia.
     */
    void Init(const EngineConfig& config);

    /**
     * @brief Uruchamia glowna petle gry.
     * @param game Obiekt logiki gry implementujacy interfejs @ref IGame.
     */
    void Run(IGame* game);

    /**
     * @brief Zatrzymuje petle gry i zamyka okno.
     */
    void Quit();

    /**
     * @brief Dodaje obiekt do sceny w bezpieczny sposob (przez kolejke).
     * @param obj Obiekt do przejecia na wlasnosc silnika.
     */
    void Instantiate(std::unique_ptr<GameObject> obj);

    /**
     * @brief Usuwa wszystkie obiekty gry i czysci kolejke tworzenia.
     */
    void ClearScene();

    /**
     * @brief Ustawia kolor tla renderowanej sceny.
     * @param color Docelowy kolor tla.
     */
    void SetBackgroundColor(Color color);

    /** @brief Zwraca referencje tylko-do-odczytu do okna SFML. */
    const sf::RenderWindow& GetWindow() const;

    /** @brief Zwraca referencje do obiektu renderera. */
    Renderer& GetRenderer() const;

    /** @brief Zwraca szerokosc aktualnego okna w pikselach. */
    unsigned int GetScreenWidth() const;

    /** @brief Zwraca wysokosc aktualnego okna w pikselach. */
    unsigned int GetScreenHeight() const;

    /** @brief Zwraca aktualny limit FPS. */
    unsigned int GetTargetFramerate() const;
};
