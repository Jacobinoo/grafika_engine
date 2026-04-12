#pragma once
#include "BaseComponent.h"
#include "../Renderer/Renderer.h"
#include <SFML/Graphics/Image.hpp>

class SpriteComponent : public BaseComponent {
private:
    sf::Image* image;
    int srcX, srcY;
    int frameW, frameH;

public:
    // Wskaznik na obrazek (najlepiej pobrany z AssetManagera) i wymiary
    SpriteComponent(sf::Image* img, int sx, int sy, int fw, int fh)
        : image(img), srcX(sx), srcY(sy), frameW(fw), frameH(fh) {}

    void Draw(Renderer& renderer) override {
        if (!gameObject || !image) return;

        // Bierzemy caly stan TRS ze swiata!
        Point2D pos = gameObject->GetWorldPosition();
        float rot = gameObject->GetWorldAngle();
        Point2D scl = gameObject->GetWorldScale();

        // Wysylamy do renderera
        renderer.DrawSprite(pos, rot, scl, *image, srcX, srcY, frameW, frameH);
    }
};