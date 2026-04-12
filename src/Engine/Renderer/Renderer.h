#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <cstdint>
#include <string>
#include "../Math/Primitives.h"

/**
 * @brief Renderer programowy oparty o bufor pikseli RGBA.
 *
 * Klasa implementuje rysowanie prymitywow geometrycznych bezposrednio do
 * tablicy bajtow, a nastepnie przesyla bufor do tekstury SFML.
 * @ingroup module_renderer
 */
class Renderer {
private:
    unsigned int width;                    /**< Szerokosc bufora renderowania. */
    unsigned int height;                   /**< Wysokosc bufora renderowania. */
    std::vector<std::uint8_t> pixelBuffer; /**< Surowy bufor pikseli w formacie RGBA. */
    sf::Texture texture;                   /**< Tekstura SFML aktualizowana z bufora. */
    sf::Sprite sprite;                     /**< Sprite prezentujacy teksture na oknie. */

public:
    /**
     * @brief Tworzy renderer o zadanych wymiarach.
     * @param w Szerokosc bufora.
     * @param h Wysokosc bufora.
     */
    Renderer(unsigned int w, unsigned int h);

    /**
     * @brief Ustawia pojedynczy piksel w buforze.
     * @param x Wspolrzedna X.
     * @param y Wspolrzedna Y.
     * @param color Kolor RGBA piksela.
     */
    void PutPixel(int x, int y, sf::Color color);

    /**
     * @brief Czysci caly bufor jednym kolorem.
     * @param color Kolor tla.
     */
    void Clear(sf::Color color);

    /** @brief Rysuje kontur odcinka. */
    void Draw(const LineSegment& line, sf::Color color);
    /** @brief Rysuje kontur trojkata. */
    void Draw(const Triangle& triangle, sf::Color color);
    /** @brief Rysuje kontur prostokata. */
    void Draw(const Rectangle& rect, sf::Color color);
    /** @brief Rysuje kontur wielokata. */
    void Draw(const Polygon& polygon, sf::Color color);
    /** @brief Rysuje kontur okregu. */
    void Draw(const Circle& circle, sf::Color color);
    void Draw(const Ellipse& ellipse, sf::Color color);

    /** @brief Wypelnia trojkat. */
    void Fill(const Triangle& triangle, sf::Color color);
    /** @brief Wypelnia prostokat. */
    void Fill(const Rectangle& rect, sf::Color color);
    /** @brief Wypelnia wielokat algorytmem scanline. */
    void Fill(const Polygon& polygon, sf::Color color);
    /** @brief Wypelnia kolo algorytmem scanline opartym o Bresenhama. */
    void Fill(const Circle& circle, sf::Color color);
    void Fill(const Ellipse& ellipse, sf::Color color);

    /**
     * @brief Rysuje tekst bitmapowy z atlasu znakow 16x16.
     * @param text Napis ASCII.
     * @param startX Pozycja startowa X.
     * @param startY Pozycja startowa Y.
     * @param fontImage Obraz atlasu czcionki.
     * @param scale Skala rysowania znaku.
     */
    void DrawBitmapText(const std::string& text, int startX, int startY, const sf::Image& fontImage, float scale = 1.0f);

    void DrawSprite(const Point2D& pos, float angle, const Point2D& scale, const sf::Image& image, int srcX, int srcY, int frameW, int frameH);

    /**
     * @brief Przesyla bufor pikseli do tekstury i rysuje go w oknie.
     * @param window Okno docelowe SFML.
     */
    void RenderToWindow(sf::RenderWindow& window);
};