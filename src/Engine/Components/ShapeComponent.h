#pragma once
#include "BaseComponent.h"
#include "../Math/Primitives.h"
#include "../Renderer/Renderer.h"
#include <variant>

/**
 * @brief Wariant przechowujacy jedna z obslugiwanych figur geometrycznych.
 * @ingroup module_components
 */
using ShapeType = std::variant<LineSegment, Triangle, Rectangle, Polygon, Circle>;

/**
 * @brief Komponent wizualny rysujacy figure geometryczna powiazana z GameObject.
 *
 * Figura jest zdefiniowana lokalnie, a podczas rysowania transformowana
 * do przestrzeni swiata (rotacja + translacja).
 * @ingroup module_components
 */
class ShapeComponent : public BaseComponent {
private:
    ShapeType shape; /**< Figura lokalna przypieta do obiektu. */
    sf::Color color; /**< Kolor rysowania figury. */
    bool isFilled;   /**< Flaga wyboru trybu: kontur lub wypelnienie. */

public:
    /**
     * @brief Tworzy komponent ksztaltu.
     * @param s Figura lokalna.
     * @param c Kolor figury.
     * @param filled Czy figura ma byc wypelniona.
     */
    ShapeComponent(ShapeType s, sf::Color c, bool filled = false)
        : shape(s), color(c), isFilled(filled) {}

    /**
     * @brief Ustawia nowy kolor figury.
     * @param newColor Docelowy kolor.
     */
    void SetColor(sf::Color newColor) {
        color = newColor;
    }

    /**
     * @brief Zwraca aktualny kolor figury.
     * @return Kolor figury.
     */
    sf::Color GetColor() const {
        return color;
    }

    /**
     * @brief Podmienia przechowywana figure.
     * @param newShape Nowa figura lokalna.
     */
    void SetShape(ShapeType newShape) {
        shape = newShape;
    }

    /**
     * @brief Rysuje figure po transformacji do przestrzeni swiata.
     * @param renderer Referencja do renderera.
     */
    void Draw(Renderer& renderer) override {
        if (!gameObject) return;
        
        Point2D pos = gameObject->GetWorldPosition();
        float rot = gameObject->GetWorldAngle();

        // Dla kazdego typu figury wykonujemy ten sam pipeline:
        // 1) transformacja lokalna -> swiat,
        // 2) wybor Draw/Fill.
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, Circle>) {
                // Kolo: obracamy tylko srodek lokalny i przesuwamy do pozycji obiektu.
                Point2D worldCenter = arg.center.Rotate(rot) + pos;
                Circle worldCircle(worldCenter, arg.radius);
                
                if (isFilled) renderer.Fill(worldCircle, color);
                else renderer.Draw(worldCircle, color);
            }
            else if constexpr (std::is_same_v<T, Polygon>) {
                std::vector<Point2D> worldVerts;
                for (const auto& v : arg.vertices) {
                    worldVerts.push_back(v.Rotate(rot) + pos);
                }
                Polygon worldPoly(worldVerts);
                
                if (isFilled) renderer.Fill(worldPoly, color);
                else renderer.Draw(worldPoly, color);
            }
            else if constexpr (std::is_same_v<T, Rectangle>) {
                // Po rotacji prostokat osiowy staje sie czworokatem.
                // Dlatego konwertujemy go do wielokata i rysujemy ogolnym algorytmem.
                std::vector<Point2D> verts = {
                    arg.topLeft,
                    Point2D(arg.topLeft.x + arg.width, arg.topLeft.y),
                    Point2D(arg.topLeft.x + arg.width, arg.topLeft.y + arg.height),
                    Point2D(arg.topLeft.x, arg.topLeft.y + arg.height)
                };
                
                std::vector<Point2D> worldVerts;
                for (const auto& v : verts) {
                    worldVerts.push_back(v.Rotate(rot) + pos);
                }
                
                Polygon worldPoly(worldVerts);
                if (isFilled) renderer.Fill(worldPoly, color);
                else renderer.Draw(worldPoly, color);
            }
            else if constexpr (std::is_same_v<T, Triangle>) {
                // Trojkat: transformujemy trzy lokalne wierzcholki.
                Point2D p1 = arg.p1.Rotate(rot) + pos;
                Point2D p2 = arg.p2.Rotate(rot) + pos;
                Point2D p3 = arg.p3.Rotate(rot) + pos;
                
                Triangle worldTri(p1, p2, p3);
                
                if (isFilled) renderer.Fill(worldTri, color);
                else renderer.Draw(worldTri, color);
            }
            else if constexpr (std::is_same_v<T, LineSegment>) {
                // Odcinek: transformujemy dwa konce.
                Point2D p1 = arg.p1.Rotate(rot) + pos;
                Point2D p2 = arg.p2.Rotate(rot) + pos;
                
                LineSegment worldLine(p1, p2);
                
                // Odcinek jest figura 1D, wiec wypelnienie nie ma sensu.
                renderer.Draw(worldLine, color);
            }
            
        }, shape);
    }
};