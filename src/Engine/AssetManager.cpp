#include "AssetManager.h"
#include <iostream>

// Inicjalizacja statycznych słowników
std::unordered_map<std::string, sf::Image> AssetManager::images;

void AssetManager::LoadImage(const std::string& name, const std::string& filepath) {
    sf::Image img;
    if (img.loadFromFile(filepath)) {
        images[name] = std::move(img);
    } else {
        std::cerr << "[AssetManager] Nie udalo sie zaladowac grafiki: " << filepath << std::endl;
    }
}

sf::Image* AssetManager::GetImage(const std::string& name) {
    auto it = images.find(name);
    if (it != images.end()) {
        return &it->second; // Zwracamy wskaźnik do istniejącego obiektu
    }
    std::cerr << "[AssetManager] OSTRZEZENIE: Brak grafiki o nazwie '" << name << "'!" << std::endl;
    return nullptr;
}

void AssetManager::Clear() {
    images.clear();
}
