#pragma once
#include "BaseComponent.h"
#include "../Renderer/Renderer.h"
#include <SFML/Graphics/Image.hpp>
#include <unordered_map>
#include <string>

// NOWOŚĆ: Struktura przechowująca przepis na konkretną animację
struct AnimationState {
    int startCol;
    int startRow;
    int totalFrames;
    int ticksPerFrame;
    bool isLooping;
};

class AnimatedSpriteComponent : public BaseComponent {
private:
    sf::Image* image;
    int frameW, frameH;
    
    // NOWOŚĆ: Słownik wszystkich animacji dodanych do tego obiektu!
    std::unordered_map<std::string, AnimationState> animations;
    
    // Zmienne stanu bieżącej animacji
    std::string currentAnimName = "";
    int currentFrame = 0;
    int tickCounter = 0;
    bool isPlaying = false;

public:
    // Zmieniony konstruktor (nie podajemy już tu rzędów i klatek!)
    AnimatedSpriteComponent(sf::Image* img, int fWidth, int fHeight)
        : image(img), frameW(fWidth), frameH(fHeight) {}

    // --- NOWE METODY ---

    // Funkcja dla twórcy gry do definiowania animacji
    void AddAnimation(const std::string& name, int startCol, int startRow, int frames, int speed, bool loop = true) {
        animations[name] = {startCol, startRow, frames, speed, loop};
    }

    void Play(const std::string& name) {
        if (currentAnimName == name && isPlaying) return; 
        if (animations.find(name) == animations.end()) return;

        currentAnimName = name;

        const AnimationState& anim = animations[name];
        currentFrame = (anim.totalFrames > 1) ? 1 : 0; 
        
        tickCounter = 0;
        isPlaying = true;
    }

    void Stop() {
        isPlaying = false;
        currentFrame = 0; // Wracamy do klatki spoczynkowej
    }

    void Update() override {
        if (!isPlaying || currentAnimName.empty()) return;

        // Pobieramy dane obecnej animacji ze słownika
        const AnimationState& anim = animations[currentAnimName];

        tickCounter++;
        if (tickCounter >= anim.ticksPerFrame) {
            tickCounter = 0;
            currentFrame++;
            
            if (currentFrame >= anim.totalFrames) {
                if (anim.isLooping) currentFrame = 0;
                else currentFrame = anim.totalFrames - 1;
            }
        }
    }

    void Draw(Renderer& renderer) override {
        if (!gameObject || !image || currentAnimName.empty()) return;

        Point2D pos = gameObject->GetWorldPosition();
        float rot = gameObject->GetWorldAngle();
        Point2D scl = gameObject->GetWorldScale();

        // Pobieramy dane o rzędzie i kolumnie z naszej struktury
        const AnimationState& anim = animations[currentAnimName];

        // Wyliczamy, skąd wyciąć obrazek
        int srcX = (anim.startCol + currentFrame) * frameW;
        int srcY = anim.startRow * frameH;

        renderer.DrawSprite(pos, rot, scl, *image, srcX, srcY, frameW, frameH);
    }
};