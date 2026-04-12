#include "../Engine/IGame.h"
#include "../Engine/Engine.h"
#include "../Engine/Input/Input.h"
#include "../Engine/AssetManager.h"
#include "Box.h"
#include "Player.h"
#include "Player2.h"
#include "Ufo.h"
#include "Poly.h"

/**
 * @brief Przykladowa implementacja gry demonstracyjnej.
 * @ingroup module_demo
 */
class MyGame: public IGame {
private:
    /** @brief Aktualny kolor tla sceny. */
    Color currentColor = Color::Cyan;

public:
    /**
     * @brief Inicjalizuje akcje wejscia i tworzy obiekty sceny startowej.
     */
    void Start() override {
        AssetManager::LoadImage("Tree", "src/DemoMyGame/Assets/Sprites/tree1.png");
        AssetManager::LoadImage("Player", "src/DemoMyGame/Assets/Sprites/Player.png");
        AssetManager::LoadImage("Player2", "src/DemoMyGame/Assets/Sprites/Player2.png");
        AssetManager::LoadImage("Grass", "src/DemoMyGame/Assets/Sprites/Grass.png");


        // Mapowanie akcji logicznych na klawisze uproszcza dalsza logike gry.
        Input::BindMouseAction("Click", MouseCode::Left);
        Input::BindAction("ChangeColor", KeyCode::K);

        Input::BindAction("MoveLeft", KeyCode::Left);
        Input::BindAction("MoveRight", KeyCode::Right);

        // Input::BindAction("UfoMoveLeft", KeyCode::A);
        // Input::BindAction("UfoMoveRight", KeyCode::D);

        Input::BindAction("PlayerMoveLeft", KeyCode::A);
        Input::BindAction("PlayerMoveRight", KeyCode::D);
        Input::BindAction("PlayerMoveUp", KeyCode::W);
        Input::BindAction("PlayerMoveDown", KeyCode::S);
        Input::BindAction("PlayerRun", KeyCode::LShift);

        Input::BindAction("Player2MoveLeft", KeyCode::Left);
        Input::BindAction("Player2MoveRight", KeyCode::Right);
        Input::BindAction("Player2MoveUp", KeyCode::Up);
        Input::BindAction("Player2MoveDown", KeyCode::Down);


        Engine &engine = Engine::GetInstance();
        engine.SetBackgroundColor(currentColor);

        /** @brief Szerokosc okna potrzebna do rozmieszczenia obiektow. */
        int screenW = engine.GetScreenWidth();
        /** @brief Wysokosc okna potrzebna do rozmieszczenia obiektow. */
        int screenH = engine.GetScreenHeight();

        for (int x = 0; x < screenW+16; x += 16) {
            for (int y = 0; y < screenH+16; y += 16) {
                engine.Instantiate(
                    std::make_unique<Box>(x, y)
                );
            }
        }
        engine.Instantiate(
            std::make_unique<Box>((screenW/2), screenH-65)
        );

        engine.Instantiate(
            std::make_unique<Box>((screenW/2-300), screenH-65)
        );

        // engine.Instantiate(
        //     std::make_unique<Poly>((screenW/2)-600, screenH/2)
        // );

        // engine.Instantiate(
        //     std::make_unique<Ufo>((screenW/2)-50, screenH/2)
        // );

        engine.Instantiate(
            std::make_unique<Player>(screenW/2, screenH/2)
        );

        engine.Instantiate(
            std::make_unique<Player2>(screenW/2+400, screenH/2+400)
        );
    }

    /**
     * @brief Aktualizuje logike gry w kazdej klatce.
     */
    void Update() override {
        // Prosty automat 2-stanowy: po nacisnieciu klawisza K przelaczamy kolor tla.
        Engine &engine = Engine::GetInstance();
        if(Input::IsActionJustPressed("ChangeColor")) {
            if(currentColor == Color::Blue)
                currentColor = Color::Green;
            else currentColor = Color::Blue;
        }
        engine.SetBackgroundColor(currentColor);
    }

};