#include "Engine.h"
#include "IGame.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <algorithm>
#include <iostream>

/**
 * @brief Inicjalizuje glowny kontekst uruchomieniowy silnika.
 */
void Engine::Init(const EngineConfig& config) {
    // 1) Ustawienia domyslne okna.
    unsigned int windowStyle = sf::Style::Default;
    sf::State windowState = sf::State::Windowed;

    // 2) Parametry aktualnego trybu pulpitu.
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    // Tworzymy tryb bazowy z zadanej szerokosci/wysokosci.
    sf::VideoMode videoMode({(unsigned int)config.width, (unsigned int)config.height}, desktop.bitsPerPixel);

    // 3) Obsluga trybu pelnoekranowego.
    if (config.windowMode == WindowMode::FULLSCREEN) {
        windowState = sf::State::Fullscreen;
        bool isModeSupported = false;
        
        // Sprawdzamy, czy zadany tryb jest wspierany przez monitor.
        std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();

        for (const auto& mode : modes) {
            if (mode.size.x == config.width && mode.size.y == config.height) {
                isModeSupported = true;
                videoMode = mode;
                break;
            }
        }

        // Fallback: przechodzimy do natywnego trybu pulpitu.
        if (!isModeSupported) {
            std::cout << "[Engine] OSTRZEZENIE: Rozdzielczosc " << config.width << "x" << config.height
                      << " nie jest obslugiwana w Fullscreen na tym monitorze!" << std::endl;
            
            std::cout << "[Engine] Wymuszam natywna rozdzielczosc pulpitu: "
                      << desktop.size.x << "x" << desktop.size.y << std::endl;

            videoMode = desktop; 
        }
    }
    else if (config.windowMode == WindowMode::BORDERLESS) {
        windowStyle = sf::Style::None;
    }

    // 4) Tworzenie okna.
    window = std::make_unique<sf::RenderWindow>(videoMode, config.title, windowStyle, windowState);

    // 5) Konfiguracja FPS.
    window->setVerticalSyncEnabled(false);
    window->setFramerateLimit(config.framerateLimit);
    targetFramerate = config.framerateLimit;

    if(config.framerateLimit == 0) {
        window->setFramerateLimit(60);
        targetFramerate = 60;
    }

    // 6) Inicjalizacja renderera programowego.
    renderer = std::make_unique<Renderer>(videoMode.size.x, videoMode.size.y);

    // Rezerwacja redukuje realokacje i fragmentacje pamieci podczas tworzenia obiektow.
    gameObjects.reserve(1000);
    objectQueue.reserve(100);

    isRunning = true;
}

/**
 * @brief Uruchamia petle glowna i cykl zycia gry.
 */
void Engine::Run(IGame* game) {
    if(game) game->Start();

    while (window->isOpen() && isRunning) {
        // 1) Obsluga zdarzen systemowych.
        while (const std::optional event = window->pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                Quit();
            }
        }

        // 2) Snapshot wejscia dla tej klatki.
        Input::Update();

        // 3) Kod gry uzytkownika.
        if (game) game->Update();

        // 4) Aktualizacja aktywnych obiektow.
        for (auto& obj : gameObjects) if (obj->isActive) obj->Update();

        // Kolejka obiektow zapobiega invalidacji iteratorow podczas petli Update().
        for (auto& newObj : objectQueue) {
            gameObjects.push_back(std::move(newObj));
        }
        objectQueue.clear();

        // Remove-erase idiom: liniowe usuniecie obiektow oznaczonych jako nieaktywne.
        gameObjects.erase(
            std::remove_if(gameObjects.begin(), gameObjects.end(),
                [](const std::unique_ptr<GameObject>& obj) { return !obj->isActive; }),
            gameObjects.end()
        );

        // 5) Render klatki.
        window->clear(currentBackgroundColor);
        renderer->Clear(currentBackgroundColor);

        for (auto& obj : gameObjects) {
            obj->Render(*renderer);
        }

        renderer->RenderToWindow(*window);
        window->display();
    }

    // Sprzatanie po zakonczeniu petli.
    ClearScene();
    if (window) window.reset();
    if (renderer) renderer.reset();
}

/**
 * @brief Ustawia kolor tla sceny.
 */
void Engine::SetBackgroundColor(Color color) {
    currentBackgroundColor = color;
}

/**
 * @brief Dodaje obiekt do kolejki tworzenia.
 */
void Engine::Instantiate(std::unique_ptr<GameObject> obj) {
    objectQueue.push_back(std::move(obj));
}

/**
 * @brief Zatrzymuje petle silnika i zamyka okno.
 */
void Engine::Quit() {
    isRunning = false;
    if (window->isOpen()) {
        window->close();
    }
}

/**
 * @brief Czysci scene i kolejke obiektow.
 */
void Engine::ClearScene() {
    gameObjects.clear();
    objectQueue.clear();
}

/** @brief Zwraca referencje do okna. */
const sf::RenderWindow& Engine::GetWindow() const { return *window; }
/** @brief Zwraca referencje do renderera. */
Renderer& Engine::GetRenderer() const { return *renderer; }
/** @brief Zwraca szerokosc okna. */
unsigned int Engine::GetScreenWidth() const { return window->getSize().x;};
/** @brief Zwraca wysokosc okna. */
unsigned int Engine::GetScreenHeight() const { return window->getSize().y;};
/** @brief Zwraca docelowy limit FPS. */
unsigned int Engine::GetTargetFramerate() const { return targetFramerate; }
