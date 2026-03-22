#include "Engine.h"
#include "IGame.h"
#include "Input/Input.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <iostream>
//KACPER
void Engine::Init(const EngineConfig& config) {
    // 1. USTALENIE POCZĄTKOWYCH PARAMETRÓW OKNA
    // Style::Default to okno z ramką, tytułem i przyciskami (zamknij, minimalizuj)
    unsigned int windowStyle = sf::Style::Default;
    // State::Windowed oznacza, że okno nie zajmuje domyślnie całego ekranu
    sf::State windowState = sf::State::Windowed;

    // 2. POBIERANIE DANYCH O MONITORZE
    // Pobieramy parametry aktualnego trybu pulpitu (rozdzielczość i głębia kolorów w bitach)
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    // Tworzymy docelowy tryb wideo na podstawie danych z konfiguracji (szerokość, wysokość)
    // Rzutujemy na (unsigned int), ponieważ SFML wymaga liczb dodatnich dla rozmiaru
    sf::VideoMode videoMode({(unsigned int)config.width, (unsigned int)config.height}, desktop.bitsPerPixel);

    // 3. LOGIKA TRYBU PEŁNOEKRANOWEGO (FULLSCREEN)
    if (config.windowMode == WindowMode::FULLSCREEN) {
        windowState = sf::State::Fullscreen; // Zmieniamy stan okna na pełny ekran
        bool isModeSupported = false;
        
        // Pobieramy listę wszystkich rozdzielczości wspieranych przez monitor w trybie pełnoekranowym
        std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();

        // Pętla sprawdzająca: Czy rozdzielczość podana przez użytkownika w config jest na liście wspieranych?
        for (const auto& mode : modes) {
            if (mode.size.x == config.width && mode.size.y == config.height) {
                isModeSupported = true;
                videoMode = mode; // Jeśli znaleziono pasujący tryb, przypisujemy go
                break;
            }
        }

        // Jeśli monitor NIE wspiera żądanej rozdzielczości (zabezpieczenie przed błędem sterownika)
        if (!isModeSupported) {
            std::cout << "[Engine] OSTRZEZENIE: Rozdzielczosc " << config.width << "x" << config.height
                      << " nie jest obslugiwana w Fullscreen na tym monitorze!" << std::endl;
            
            // Wymuszamy bezpieczną rozdzielczość pulpitu, aby gra w ogóle się uruchomiła
            std::cout << "[Engine] Wymuszam natywna rozdzielczosc pulpitu: "
                      << desktop.size.x << "x" << desktop.size.y << std::endl;

            videoMode = desktop; 
        }
    }
    // 4. LOGIKA TRYBU BEZRAMKOWEGO (BORDERLESS)
    else if (config.windowMode == WindowMode::BORDERLESS) {
        // Style::None usuwa ramkę systemową i pasek tytułu
        windowStyle = sf::Style::None;
    }

    // 5. KREACJA OKNA
    // std::make_unique tworzy bezpieczny, inteligentny wskaźnik na okno RenderWindow
    // Parametry: (rozmiar/kolory, tytuł okna, styl ramki, stan okna)
    window = std::make_unique<sf::RenderWindow>(videoMode, config.title, windowStyle, windowState);

    // 6. KONFIGURACJA PŁYNNOŚCI (FPS)
    // Wyłączamy VSync, aby ręcznie kontrolować limit klatek (mniejszy input lag w niektórych grach)
    window->setVerticalSyncEnabled(false);
    
    // Ustawiamy limit klatek na sekundę z pliku konfiguracyjnego
    window->setFramerateLimit(config.framerateLimit);
    targetFramerate = config.framerateLimit;

    // Zabezpieczenie: jeśli ktoś poda limit 0, ustawiamy standardowe 60 FPS, żeby gra nie "pędziła" bez ograniczeń
    if(config.framerateLimit == 0) {
        window->setFramerateLimit(60);
        targetFramerate = 60;
    }

    // Flaga informująca, że inicjalizacja przebiegła pomyślnie i silnik może zacząć pracę
    isRunning = true;
}
//KACPER

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
