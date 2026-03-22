#include "DemoMyGame/MyGame.h"
#include "Engine/Engine.h"

//Kuba: Głowna petla gry, Obsluga klawiatury i mszy
//Łukasz: Zamkniecie gry, parametryzowanie innych parametrow
//Kacper: Inicjjacja biblioteki, parametryzofanie trybu graficznego
//4: Czyszczenie ekranu

int main() {

    EngineConfig config;
    config.width = 1920;
    config.height = 1080;
    config.title = "Moja Gra";
    config.windowMode = WindowMode::FULLSCREEN;
    config.framerateLimit = 30;

	Engine& engine = Engine::GetInstance();
	engine.Init(config);

    MyGame game;

    engine.Run(&game);

    return 0;
}
