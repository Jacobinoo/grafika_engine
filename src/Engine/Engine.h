#pragma once
#include "IGame.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <memory>
#include <string>

using Color = sf::Color;

// Tryby wyświetlania
enum class WindowMode {
    WINDOWED,     // Zwykłe okno z paskiem
    FULLSCREEN,   // Pełny ekran
    BORDERLESS    // Okno bez ramek
};

struct EngineConfig {
    int width = 1280;
    int height = 720;
    std::string title = "Engine Window";
    WindowMode windowMode = WindowMode::WINDOWED;
    int framerateLimit = 60;
};

class Engine {
private:
    std::unique_ptr<sf::RenderWindow> window;
    unsigned int targetFramerate = 60;

    bool isRunning = false;

    // Singleton
    Engine() = default;
    ~Engine() = default;

    // Blokada kopiowania silnika
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

public:
    static Engine& GetInstance() {
        static Engine instance;
        return instance;
    }

    void Init(const EngineConfig& config);
    void Run(IGame* game);
    void Quit();

    // Wywołuje to, żeby wyczyścić scenę do koloru
    void ClearScene(sf::Color color);

    // Gettery
    const sf::RenderWindow& GetWindow() const;
};
