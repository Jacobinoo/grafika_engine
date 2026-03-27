#include "DemoMyGame/MyGame.h"
#include "Engine/Engine.h"

/**
 * @brief Punkt startowy aplikacji.
 * @return Kod wyjscia procesu.
 */
int main() {
    /** @brief Konfiguracja startowa silnika. */
    EngineConfig config;

    /** @brief Szerokosc okna gry. */
    config.width = 1920;
    /** @brief Wysokosc okna gry. */
    config.height = 1080;
    /** @brief Tytul okna aplikacji. */
    config.title = "Moja Gra";
    /** @brief Tryb uruchomienia okna (pelny ekran). */
    config.windowMode = WindowMode::FULLSCREEN;
    /** @brief Limit klatek na sekunde. */
    config.framerateLimit = 30;

	/** @brief Referencja do singletonu silnika. */
	Engine& engine = Engine::GetInstance();
	engine.Init(config);

    /** @brief Instancja logiki gry. */
    MyGame game;

    engine.Run(&game);

    return 0;
}
