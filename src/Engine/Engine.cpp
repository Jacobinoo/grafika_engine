#include "Engine.h"
#include "IGame.h"
#include "Input/Input.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <iostream>

void Engine::Init(const EngineConfig& config) {
    unsigned int windowStyle = sf::Style::Default;
    sf::State windowState = sf::State::Windowed;

    // Pobieramy informacje o pulpicie (do głębi kolorów bitsPerPixel)
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    // Domyślnie używamy rozdzielczości z konfiguracji i głębi kolorów pulpitu
    sf::VideoMode videoMode({(unsigned int)config.width, (unsigned int)config.height}, desktop.bitsPerPixel);

    if (config.windowMode == WindowMode::FULLSCREEN) {
        windowState = sf::State::Fullscreen;
        bool isModeSupported = false;
        std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();

        // Czy monitor obsługuje żądaną rozdzielczość?
        for (const auto& mode : modes) {
            if (mode.size.x == config.width && mode.size.y == config.height) {
                isModeSupported = true;
                videoMode = mode; // Używamy tego trybu
                break;
            }
        }

        if (!isModeSupported) {
            std::cout << "[Engine] OSTRZEZENIE: Rozdzielczosc " << config.width << "x" << config.height
                      << " nie jest obslugiwana w Fullscreen na tym monitorze!" << std::endl;
            std::cout << "[Engine] Wymuszam natywna rozdzielczosc pulpitu: "
                      << desktop.size.x << "x" << desktop.size.y << std::endl;

            videoMode = desktop; // Nadpisujemy bezpiecznym trybem pulpitu
        }
    }
    else if (config.windowMode == WindowMode::BORDERLESS) {
        windowStyle = sf::Style::None;
    }

    window = std::make_unique<sf::RenderWindow>(videoMode, config.title, windowStyle, windowState);

    window->setVerticalSyncEnabled(false);
    window->setFramerateLimit(config.framerateLimit);
    targetFramerate = config.framerateLimit;

    if(config.framerateLimit == 0) {
        window->setFramerateLimit(60);
        targetFramerate = 60;
    }

    isRunning = true;
}

void Engine::Run(IGame* game) {
    if(game) game->Start();

    while (window->isOpen() && isRunning) {
        // Obsluga zdarzen okna
        while (const std::optional event = window->pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                Quit();
            }
        }

        // Obsługa wejscia
        Input::Update();

        // Wstrzykiwanie logiki twórcy gry
        if (game) game->Update();

        window->display();
    }
    if (window) window.reset();
}

void Engine::Quit() {
    isRunning = false;
    if (window->isOpen()) {
        window->close();
    }
}

void Engine::ClearScene(sf::Color color) {
    window->clear(color);
}

const sf::RenderWindow& Engine::GetWindow() const { return *window; }
