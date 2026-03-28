#include "../Engine/IGame.h"
#include "../Engine/Engine.h"
#include "../Engine/Input/Input.h"
#include "Box.h"
#include "Ship.h"
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
        // Mapowanie akcji logicznych na klawisze uproszcza dalsza logike gry.
        Input::BindMouseAction("Click", MouseCode::Left);
        Input::BindAction("ChangeColor", KeyCode::K);

        Input::BindAction("MoveLeft", KeyCode::Left);
        Input::BindAction("MoveRight", KeyCode::Right);

        Input::BindAction("UfoMoveLeft", KeyCode::A);
        Input::BindAction("UfoMoveRight", KeyCode::D);

        Input::BindAction("BoxMoveLeft", KeyCode::V);
        Input::BindAction("BoxMoveRight", KeyCode::B);


        Engine &engine = Engine::GetInstance();
        engine.SetBackgroundColor(currentColor);

        /** @brief Szerokosc okna potrzebna do rozmieszczenia obiektow. */
        int screenW = engine.GetScreenWidth();
        /** @brief Wysokosc okna potrzebna do rozmieszczenia obiektow. */
        int screenH = engine.GetScreenHeight();

        engine.Instantiate(
            std::make_unique<Ship>(screenW/2, screenH/2)
        );

        engine.Instantiate(
            std::make_unique<Ufo>((screenW/2)-50, screenH/2)
        );

        engine.Instantiate(
            std::make_unique<Box>((screenW/2)-200, screenH/2)
        );

        engine.Instantiate(
            std::make_unique<Poly>((screenW/2)-600, screenH/2)
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