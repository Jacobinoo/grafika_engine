#pragma once
#include <SFML/Graphics/Image.hpp>
#include <unordered_map>
#include <string>

class AssetManager {
private:
    // Słowniki przechowujące nasze zasoby pod tekstowymi nazwami (np. "Laser")
    static std::unordered_map<std::string, sf::Image> images;

public:
    // Funkcje do ładowania plików do pamięci
    static void LoadImage(const std::string& name, const std::string& filepath);

    // Funkcje do pobierania wskaźników w dowolnym miejscu w grze
    static sf::Image* GetImage(const std::string& name);

    // Opcjonalne czyszczenie pamięci przed wyjściem
    static void Clear();
};
