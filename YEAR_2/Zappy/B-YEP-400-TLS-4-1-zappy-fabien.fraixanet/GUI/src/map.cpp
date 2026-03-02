#include "Map.hpp"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>
#include "Player.hpp"

void Map::initialize(int w, int h)
{
    width = w;
    height = h;

    loadPlayerTextures();
    loadResourceTextures();

    std::vector<std::string> textureFiles = {
        "GUI/assets/cellule.png", "GUI/assets/bat1.png", "GUI/assets/bat2.png",
        "GUI/assets/bat3.png", "GUI/assets/bat4.png", "GUI/assets/bat5.png"
    };
    resourceOffsets = {
        {"food", sf::Vector2f(10.f, 10.f)},
        {"linemate", sf::Vector2f(30.f, 10.f)},
        {"deraumere", sf::Vector2f(50.f, 10.f)},
        {"sibur", sf::Vector2f(10.f, 30.f)},
        {"mendiane", sf::Vector2f(30.f, 30.f)},
        {"phiras", sf::Vector2f(50.f, 30.f)},
        {"thystame", sf::Vector2f(30.f, 50.f)},
    };
    textures.clear();
    for (const auto& file : textureFiles) {
        sf::Texture tex;
        if (!tex.loadFromFile(file)) {
            std::cerr << "Failed to load texture: " << file << std::endl;
        }
        textures.push_back(tex);
    }
    std::vector<std::pair<int, int>> positions;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            positions.emplace_back(x, y);
        }
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(positions.begin(), positions.end(), g);
    buildingPositions.clear();
    buildingIndices.clear();
    size_t limit = std::min<size_t>(5, positions.size());
    for (size_t i = 0; i < limit; ++i) {
        buildingPositions.push_back(positions[i]);
        buildingIndices.push_back(i + 1);
    }
}

void Map::loadResourceTextures()
{
    if (!font.loadFromFile("GUI/assets/font.ttf")) {
        std::cerr << "Erreur: Impossible de charger la police GUI/assets/font.ttf" << std::endl;
    }

    std::map<std::string, std::string> textureFiles = {
        {"food", "GUI/assets/ressources/food.png"},
        {"linemate", "GUI/assets/ressources/linemate.png"},
        {"deraumere", "GUI/assets/ressources/deraumere.png"},
        {"sibur", "GUI/assets/ressources/sibur.png"},
        {"mendiane", "GUI/assets/ressources/mendiane.png"},
        {"phiras", "GUI/assets/ressources/phiras.png"},
        {"thystame", "GUI/assets/ressources/thystame.png"},
    };
    resourceTextures.clear();
    for (const auto& [name, path] : textureFiles) {
        sf::Texture tex;
        if (!tex.loadFromFile(path)) {
            std::cerr << "Erreur: Impossible de charger " << path << std::endl;
        } else {
            resourceTextures[name] = tex;
        }
    }
}

void Map::setResourcesAt(int x, int y, const std::vector<int>& resources) {
    std::cout << "setResourcesAt(" << x << ", " << y << "): ";
    for (auto r : resources)
        std::cout << r << " ";
    std::cout << std::endl;

    mapResources[{x, y}] = resources;
}

void Map::loadPlayerTextures()
{
    std::map<PlayerOrientation, std::string> orientationFiles = {
        {PlayerOrientation::North, "front.png"},
        {PlayerOrientation::East, "right.png"},
        {PlayerOrientation::South, "down.png"},
        {PlayerOrientation::West, "left.png"}
    };
    for (int level = 1; level <= 8; ++level) {
        for (auto const& [orientation, filename] : orientationFiles) {
            std::string path = "GUI/assets/level" + std::to_string(level) + "/" + filename;
            sf::Texture texture;
            if (texture.loadFromFile(path)) {
                _playerTextures[level][orientation] = texture;
            } else {
                std::cerr << "Failed to load texture: " << path << std::endl;
            }
        }
    }
    if (!_defaultTexture.loadFromFile("GUI/assets/level1/front.png")) {
         std::cerr << "Failed to load default texture" << std::endl;
    }
}

sf::Texture& Map::getPlayerTexture(int level, PlayerOrientation orientation)
{
    if (_playerTextures.count(level) && _playerTextures.at(level).count(orientation)) {
        return _playerTextures.at(level).at(orientation);
    }
    return _defaultTexture;
}

void Map::removePlayer(int id)
{
    _players.erase(id);
}

void Map::addPlayer(int id, int x, int y, PlayerOrientation orientation, int level)
{
    auto player = std::make_unique<Player>(id, x, y, orientation, level);
    _players[id] = std::move(player);
}

void Map::updatePlayerPosition(int id, int x, int y, PlayerOrientation orientation)
{
    if (_players.find(id) != _players.end()) {
        _players[id]->setPosition(x, y);
        _players[id]->setOrientation(orientation);
    }
}


void Map::draw(sf::RenderWindow& window, int size_square, int size_x, int size_y, sf::Time elapsed)
{
    float hauteur = 4.0f;
    float gap = 0.1f;
    float scale;
    float iso_x;
    float iso_y;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            iso_x = (x - y) * (size_square / 2.0f) + size_x;
            iso_y = (x + y) * (size_square / hauteur) + size_y;

            sf::Vector2f points[4] = {
                sf::Vector2f(size_square / 2.0f, 0),
                sf::Vector2f(size_square, size_square / hauteur),
                sf::Vector2f(size_square / 2.0f, size_square / (hauteur / 2.0f)),
                sf::Vector2f(0, size_square / hauteur)
            };
            sf::Vector2f center(0, 0);
            for (int i = 0; i < 4; ++i) center += points[i];
            center /= 4.0f;
            scale = 1.0f - gap;
            for (int i = 0; i < 4; ++i)
                points[i] = center + (points[i] - center) * scale;

            sf::ConvexShape cell;
            cell.setPointCount(4);
            for (int i = 0; i < 4; ++i)
                cell.setPoint(i, points[i]);
            cell.setPosition(iso_x, iso_y);
            cell.setTexture(&textures[0]);
            cell.setOutlineThickness(1);
            cell.setOutlineColor(sf::Color::Black);
            window.draw(cell);

            /*for (size_t i = 0; i < buildingPositions.size(); ++i) {
                if (buildingPositions[i].first == x && buildingPositions[i].second == y) {
                    sf::Sprite buildingSprite;
                    buildingSprite.setTexture(textures[buildingIndices[i]]);
                    sf::FloatRect bounds = buildingSprite.getLocalBounds();
                    float scaleX = (size_square / bounds.width) * 1.4f;
                    float scaleY = ((size_square / 2.0f) / bounds.height) * 1.4f;
                    buildingSprite.setScale(scaleX, scaleY);
                    buildingSprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
                    buildingSprite.setPosition(iso_x + (size_square / 2.0f), iso_y + (size_square / 4.0f));
                    window.draw(buildingSprite);
                    break;
                }
            }*/

            if (mapResources.count({x, y})) {
                const auto& resources = mapResources.at({x, y});
                const int gridSize = 3;
                const float cellSize = size_square / 2.0f;
                const float iconScaleFactor = 1.5f;
                int itemIndex = 0;

                for (size_t i = 0; i < resources.size(); ++i) {
                    int quantity = resources[i];
                    if (quantity <= 0) continue;

                    std::string resName = resourceNames[i];
                    if (resourceTextures.find(resName) == resourceTextures.end()) continue;
                    int gridX = itemIndex % gridSize;
                    int gridY = itemIndex / gridSize;

                    sf::Sprite sprite;
                    sprite.setTexture(resourceTextures.at(resName));
                    sf::FloatRect bounds = sprite.getLocalBounds();

                    float scaleX = (cellSize / gridSize) / bounds.width * iconScaleFactor;
                    float scaleY = (cellSize / gridSize) / bounds.height * iconScaleFactor;
                    sprite.setScale(scaleX, scaleY);
                    sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

                    float posX = iso_x + (size_square / 2.0f) - (cellSize / 2.0f) + (gridX + 0.5f) * (cellSize / gridSize);
                    float posY = iso_y + (size_square / 4.0f) - (cellSize / 2.0f) + (gridY + 0.5f) * (cellSize / gridSize);
                    sprite.setPosition(posX, posY);
                    window.draw(sprite);

                    if (quantity > 1) {
                        sf::Text text;
                        text.setFont(font);
                        text.setString(std::to_string(quantity));
                        text.setCharacterSize(12);
                        text.setFillColor(sf::Color::White);
                        text.setOutlineColor(sf::Color::Black);
                        text.setOutlineThickness(1.f);
                        text.setPosition(posX + 5, posY + 5);
                        window.draw(text);
                    }
                    itemIndex++;
                }
            }
        }
    }

    for (auto const& [id, player] : _players) {
        player->update(elapsed);
        player->draw(window, *this, size_square, size_x, size_y);
    }
}

int Map::getWidth() const { return width; }
int Map::getHeight() const { return height; }
