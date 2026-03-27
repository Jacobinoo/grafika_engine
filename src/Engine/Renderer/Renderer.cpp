#include "Renderer.h"
#include <cmath>
#include <algorithm>

/**
 * @brief Tworzy bufor renderowania i zasoby SFML.
 */
Renderer::Renderer(unsigned int w, unsigned int h)
    : width(w), height(h), texture(), sprite(texture) {

    // Bufor ma 4 bajty na piksel: R, G, B, A.
    pixelBuffer.resize(width * height * 4, 0);

    // SFML 3: teksture tworzymy przez resize().
    texture.resize({width, height});

    sprite.setTextureRect(sf::IntRect({0, 0}, {(int)width, (int)height}));
}

/**
 * @brief Wpisuje pojedynczy piksel do bufora RGBA.
 */
void Renderer::PutPixel(int x, int y, sf::Color color) {
    // Ochrona przed wyjsciem poza bufor (zapobiega UB / segfault).
    if (x < 0 || x >= (int)width || y < 0 || y >= (int)height) return;

    int index = (y * width + x) * 4;
    pixelBuffer[index]     = color.r;
    pixelBuffer[index + 1] = color.g;
    pixelBuffer[index + 2] = color.b;
    pixelBuffer[index + 3] = color.a;
}

/**
 * @brief Czyci caly bufor wskazanym kolorem.
 */
void Renderer::Clear(sf::Color color) {
    for (size_t i = 0; i < pixelBuffer.size(); i += 4) {
        pixelBuffer[i]     = color.r;
        pixelBuffer[i + 1] = color.g;
        pixelBuffer[i + 2] = color.b;
        pixelBuffer[i + 3] = color.a;
    }
}

// --- RYSOWANIE KONTURÓW ---

/**
 * @brief Rysuje odcinek algorytmem Bresenhama.
 */
void Renderer::Draw(const LineSegment& line, sf::Color color) {
    int x0 = std::round(line.p1.x);
    int y0 = std::round(line.p1.y);
    int x1 = std::round(line.p2.x);
    int y1 = std::round(line.p2.y);

    int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; 

    // Bresenham wykorzystuje blad calkowitoliczbowy zamiast kosztownych operacji float.
    while (true) {
        PutPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

/** @brief Rysuje kontur trojkata trzema odcinkami. */
void Renderer::Draw(const Triangle& tri, sf::Color color) {
    Draw(LineSegment(tri.p1, tri.p2), color);
    Draw(LineSegment(tri.p2, tri.p3), color);
    Draw(LineSegment(tri.p3, tri.p1), color);
}

/** @brief Rysuje kontur prostokata. */
void Renderer::Draw(const Rectangle& rect, sf::Color color) {
    Point2D tr(rect.topLeft.x + rect.width, rect.topLeft.y);
    Point2D bl(rect.topLeft.x, rect.topLeft.y + rect.height);
    Point2D br(rect.topLeft.x + rect.width, rect.topLeft.y + rect.height);

    Draw(LineSegment(rect.topLeft, tr), color);
    Draw(LineSegment(tr, br), color);
    Draw(LineSegment(br, bl), color);
    Draw(LineSegment(bl, rect.topLeft), color);
}

/** @brief Rysuje kontur wielokata. */
void Renderer::Draw(const Polygon& poly, sf::Color color) {
    if (poly.vertices.size() < 3) return; 
    for (size_t i = 0; i < poly.vertices.size() - 1; ++i) {
        Draw(LineSegment(poly.vertices[i], poly.vertices[i + 1]), color);
    }
    Draw(LineSegment(poly.vertices.back(), poly.vertices.front()), color);
}

/**
 * @brief Rysuje kontur okregu algorytmem midpoint/Bresenham circle.
 */
void Renderer::Draw(const Circle& circle, sf::Color color) {
    int cx = std::round(circle.center.x);
    int cy = std::round(circle.center.y);
    int x = 0;
    int y = std::round(circle.radius);
    int d = 3 - 2 * y;

    // Symetria osmiu oktantow pozwala obliczac tylko 1/8 okregu.
    auto drawOctants = [&](int xc, int yc, int x, int y) {
        PutPixel(xc + x, yc + y, color); PutPixel(xc - x, yc + y, color);
        PutPixel(xc + x, yc - y, color); PutPixel(xc - x, yc - y, color);
        PutPixel(xc + y, yc + x, color); PutPixel(xc - y, yc + x, color);
        PutPixel(xc + y, yc - x, color); PutPixel(xc - y, yc - x, color);
    };

    while (y >= x) {
        drawOctants(cx, cy, x, y);
        x++;
        if (d > 0) { y--; d = d + 4 * (x - y) + 10; } 
        else { d = d + 4 * x + 6; }
    }
}

// --- WYPEŁNIANIE ---

/** @brief Wypelnia prostokat przez rasteryzacje prostego obszaru osiowego. */
void Renderer::Fill(const Rectangle& rect, sf::Color color) {
    int startX = std::max(0, (int)rect.topLeft.x);
    int startY = std::max(0, (int)rect.topLeft.y);
    int endX = std::min((int)width - 1, (int)(rect.topLeft.x + rect.width));
    int endY = std::min((int)height - 1, (int)(rect.topLeft.y + rect.height));

    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            PutPixel(x, y, color);
        }
    }
}

/**
 * @brief Zwraca orientacje punktu p1 wzgledem skierowanej krawedzi p2->p3.
 * @details To iloczyn wektorowy 2D (znak pola rownolegloboku).
 */
float EdgeSign(const Point2D& p1, const Point2D& p2, const Point2D& p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

/**
 * @brief Wypelnia trojkat testem punkt-w-trojkacie opartym o znaki krawedzi.
 */
void Renderer::Fill(const Triangle& tri, sf::Color color) {
    int minX = std::max(0, (int)std::floor(std::min({tri.p1.x, tri.p2.x, tri.p3.x})));
    int maxX = std::min((int)width - 1, (int)std::ceil(std::max({tri.p1.x, tri.p2.x, tri.p3.x})));
    int minY = std::max(0, (int)std::floor(std::min({tri.p1.y, tri.p2.y, tri.p3.y})));
    int maxY = std::min((int)height - 1, (int)std::ceil(std::max({tri.p1.y, tri.p2.y, tri.p3.y})));

    // Dla kazdego piksela z AABB sprawdzamy czy lezy po tej samej stronie kazdej krawedzi.
    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            Point2D pt((float)x, (float)y);
            bool b1 = EdgeSign(pt, tri.p1, tri.p2) < 0.0f;
            bool b2 = EdgeSign(pt, tri.p2, tri.p3) < 0.0f;
            bool b3 = EdgeSign(pt, tri.p3, tri.p1) < 0.0f;

            if ((b1 == b2) && (b2 == b3)) {
                PutPixel(x, y, color);
            }
        }
    }
}

/**
 * @brief Wypelnia wielokat algorytmem scanline.
 */
void Renderer::Fill(const Polygon& poly, sf::Color color) {
    if (poly.vertices.size() < 3) return;

    float minY = poly.vertices[0].y;
    float maxY = poly.vertices[0].y;
    for (const auto& v : poly.vertices) {
        if (v.y < minY) minY = v.y;
        if (v.y > maxY) maxY = v.y;
    }

    // Dla kazdej linii poziomej Y znajdujemy przecięcia z krawedziami wielokata,
    // sortujemy je i wypelniamy przedzialy parami.
    for (int y = (int)minY; y <= (int)maxY; ++y) {
        std::vector<int> intersections;

        for (size_t i = 0; i < poly.vertices.size(); ++i) {
            Point2D p1 = poly.vertices[i];
            Point2D p2 = poly.vertices[(i + 1) % poly.vertices.size()];

            if (p1.y == p2.y) continue;

            if (y >= std::min(p1.y, p2.y) && y < std::max(p1.y, p2.y)) {
                int intersectX = (int)(p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y));
                intersections.push_back(intersectX);
            }
        }

        std::sort(intersections.begin(), intersections.end());

        for (size_t i = 0; i + 1 < intersections.size(); i += 2) {
            for (int x = intersections[i]; x <= intersections[i + 1]; ++x) {
                PutPixel(x, y, color);
            }
        }
    }
}

/**
 * @brief Wypelnia kolo scanline'ami wyprowadzonymi z Bresenhama.
 */
void Renderer::Fill(const Circle& circle, sf::Color color) {
    int cx = std::round(circle.center.x);
    int cy = std::round(circle.center.y);
    int radius = std::round(circle.radius);

    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    // Wypelnienie realizujemy przez poziome odcinki pomiedzy parami punktow symetrycznych.
    auto drawHorizontalLine = [&](int x1, int x2, int y_row) {
        if (y_row < 0 || y_row >= (int)height) return;

        int startX = std::max(0, std::min(x1, x2));
        int endX = std::min((int)width - 1, std::max(x1, x2));

        for (int px = startX; px <= endX; ++px) {
            PutPixel(px, y_row, color);
        }
    };

    while (y >= x) {
        // Cztery scanline odpowiadaja ośmiopunktowej symetrii okregu.
        drawHorizontalLine(cx - x, cx + x, cy + y);
        drawHorizontalLine(cx - x, cx + x, cy - y);
        drawHorizontalLine(cx - y, cx + y, cy + x);
        drawHorizontalLine(cx - y, cx + y, cy - x);

        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

/**
 * @brief Rysuje tekst bitmapowy z atlasu znakow ASCII 16x16.
 */
void Renderer::DrawBitmapText(const std::string& text, int startX, int startY, const sf::Image& fontImage, float scale) {
    int cellW = 64;
    int cellH = 64;

    int scaledW = cellW * scale;
    int scaledH = cellH * scale;

    int cursorX = startX;

    for (char c : text) {
        // Kod ASCII indeksuje pozycje znaku na siatce 16x16.
        int ascii = (unsigned char)c;

        int col = ascii % 16;
        int row = ascii / 16;

        int texStartX = col * cellW;
        int texStartY = row * cellH;

        for (int sy = 0; sy < scaledH; ++sy) {
            for (int sx = 0; sx < scaledW; ++sx) {

                // Skalowanie "wsteczne": piksel docelowy mapujemy na zrodlo.
                int texX = texStartX + (sx / scale);
                int texY = texStartY + (sy / scale);

                sf::Vector2u vec2 = sf::Vector2u{(uint) texX, (uint) texY};

                sf::Color pixelColor = fontImage.getPixel(vec2);

                if (pixelColor.a > 128 && pixelColor != sf::Color::Black) {
                    PutPixel(cursorX + sx, startY + sy, pixelColor);
                }
            }
        }
        cursorX += scaledW;
    }
}

/**
 * @brief Rysuje klatke sprite'a i skaluje ja programowo.
 */
void Renderer::DrawSprite(int startX, int startY, const sf::Image& image, int srcX, int srcY, int frameW, int frameH, float scale) {
    int scaledW = frameW * scale;
    int scaledH = frameH * scale;

    // Rysujemy od srodka (origin), co upraszcza obracanie/pozycjonowanie obiektow.
    startX -= scaledW / 2;
    startY -= scaledH / 2;

    for (int sy = 0; sy < scaledH; ++sy) {
        for (int sx = 0; sx < scaledW; ++sx) {

            int texX = srcX + (sx / scale);
            int texY = srcY + (sy / scale);

            if (texX >= 0 && texX < image.getSize().x && texY >= 0 && texY < image.getSize().y) {
                sf::Vector2u vec2 = sf::Vector2u{(uint) texX, (uint) texY};
                sf::Color pixelColor = image.getPixel(vec2);

                if (pixelColor.a > 128 && pixelColor != sf::Color::Black) {
                    PutPixel(startX + sx, startY + sy, pixelColor);
                }
            }
        }
    }
}

/**
 * @brief Kopiuje bufor do tekstury i wyswietla sprite w oknie.
 */
void Renderer::RenderToWindow(sf::RenderWindow& window) {
    texture.update(pixelBuffer.data());
    window.draw(sprite);
}