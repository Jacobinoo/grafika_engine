#pragma once
#include "BaseComponent.h"
#include "../Math/Primitives.h"
#include "../Renderer/Renderer.h"
#include <variant>
#include <algorithm> // Dla std::max przy okręgu

/**
 * @brief Wariant przechowujacy jedna z obslugiwanych figur geometrycznych.
 * @ingroup module_components
 */
using ShapeType = std::variant<LineSegment, Triangle, Rectangle, Polygon, Circle, Ellipse>;

/**
 * @brief Komponent wizualny rysujacy figure geometryczna powiazana z GameObject.
 *
 * Figura jest zdefiniowana lokalnie, a podczas rysowania transformowana
 * do przestrzeni swiata (skalowanie + rotacja + translacja).
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
     * @brief Zwraca aktualny ksztalt
     * @return Kolor figury.
     */
    ShapeType GetShape() const {
        return shape;
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
        
        // Pobieramy pełny stan transformacji obiektu (T-R-S)
        Point2D pos = gameObject->GetWorldPosition();
        float rot = gameObject->GetWorldAngle();
        Point2D scl = gameObject->GetWorldScale();

        // Dla kazdego typu figury wykonujemy ten sam pipeline:
        // 1) transformacja lokalna -> swiat (Scale -> Rotate -> Translate),
        // 2) wybor Draw/Fill.
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, Circle>) {
                // Kolo: skalujemy, obracamy lokalny srodek i przesuwamy do pozycji obiektu.
                Point2D worldCenter = arg.center.Scale(scl.x, scl.y)
                                                .Rotate(rot)
                                                .Translate(pos);
                                                
                // Promien skalujemy wybierajac wieksza wartosc (by uniknac elipsy)
                float maxScale = std::max(scl.x, scl.y);
                Circle worldCircle(worldCenter, arg.radius * maxScale);
                
                if (isFilled) renderer.Fill(worldCircle, color);
                else renderer.Draw(worldCircle, color);
            }
            else if constexpr (std::is_same_v<T, Polygon>) {
                std::vector<Point2D> worldVerts;
                for (const auto& v : arg.vertices) {
                    worldVerts.push_back(v.Scale(scl.x, scl.y).Rotate(rot).Translate(pos));
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
                    worldVerts.push_back(v.Scale(scl.x, scl.y).Rotate(rot).Translate(pos));
                }
                
                Polygon worldPoly(worldVerts);
                if (isFilled) renderer.Fill(worldPoly, color);
                else renderer.Draw(worldPoly, color);
            }
            else if constexpr (std::is_same_v<T, Triangle>) {
                // Trojkat: transformujemy trzy lokalne wierzcholki (S -> R -> T).
                Point2D p1 = arg.p1.Scale(scl.x, scl.y).Rotate(rot).Translate(pos);
                Point2D p2 = arg.p2.Scale(scl.x, scl.y).Rotate(rot).Translate(pos);
                Point2D p3 = arg.p3.Scale(scl.x, scl.y).Rotate(rot).Translate(pos);
                
                Triangle worldTri(p1, p2, p3);
                
                if (isFilled) renderer.Fill(worldTri, color);
                else renderer.Draw(worldTri, color);
            }
            else if constexpr (std::is_same_v<T, LineSegment>) {
                // Odcinek: transformujemy dwa konce (S -> R -> T).
                Point2D p1 = arg.p1.Scale(scl.x, scl.y).Rotate(rot).Translate(pos);
                Point2D p2 = arg.p2.Scale(scl.x, scl.y).Rotate(rot).Translate(pos);
                
                LineSegment worldLine(p1, p2);
                
                // Odcinek jest figura 1D, wiec wypelnienie nie ma sensu.
                renderer.Draw(worldLine, color);
            }
            else if constexpr (std::is_same_v<T, Ellipse>) {
            // 1. Skalujemy lokalne promienie (wartość absolutna, by uniknąć błędów przy ujemnej skali)
            float worldRx = arg.rx * std::abs(scl.x);
            float worldRy = arg.ry * std::abs(scl.y);

            if (rot == 0.0f) {
                // SCENARIUSZ A: Elipsa nie jest obrócona. 
                // Używamy super-szybkiego algorytmu Midpoint z naszego Renderera!
                Point2D worldCenter = arg.center.Scale(scl.x, scl.y).Translate(pos);
                Ellipse worldEllipse(worldCenter, worldRx, worldRy);
                
                if (isFilled) renderer.Fill(worldEllipse, color);
                else renderer.Draw(worldEllipse, color);
            } 
            else {
                // SCENARIUSZ B: Elipsa jest OBRÓCONA.
                // Konwertujemy ją w locie na Polygon (wielokąt z 32 wierzchołków)
                std::vector<Point2D> verts;
                int segments = 32; // Złoty standard gładkości w 2D
                
                for (int i = 0; i < segments; ++i) {
                    float theta = (2.0f * M_PI * i) / segments; // Kąt w radianach
                    
                    // Punkt na obwodzie elipsy lokalnej
                    Point2D localPt(arg.center.x + arg.rx * std::cos(theta),
                                    arg.center.y + arg.ry * std::sin(theta));
                                    
                    // Przepuszczamy go przez Łańcuch S-R-T!
                    verts.push_back(localPt.Scale(scl.x, scl.y).Rotate(rot).Translate(pos));
                }
                
                Polygon worldPoly(verts); // Powstała obrócona figura!
                
                if (isFilled) renderer.Fill(worldPoly, color);
                else renderer.Draw(worldPoly, color);
            }
        }  
    }, shape);
}
};