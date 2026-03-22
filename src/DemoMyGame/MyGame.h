#include "../Engine/IGame.h"
#include "../Engine/Engine.h"


class MyGame: public IGame {

public:
    void Start() override {
        // Inicjalizacja zasobów gry
    }

    void Update() override {
        // Logika gry i renderowanie co klatke
        Engine &engine = Engine::GetInstance();
        engine.ClearScene(Color::Yellow);
    
    }

};