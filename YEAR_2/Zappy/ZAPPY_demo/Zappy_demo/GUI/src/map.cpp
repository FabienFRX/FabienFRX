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

    std::vector<std::string> textureFiles = {
        "GUI/assets/cellule.png", "GUI/assets/bat1.png", "GUI/assets/bat2.png",
        "GUI/assets/bat3.png", "GUI/assets/bat4.png", "GUI/assets/bat5.png"
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
            auto texture = std::make_unique<sf::Texture>();
            if (texture->loadFromFile(path)) {
                _playerTextures[level][orientation] = std::move(texture);
            } else {
                std::cerr << "Failed to load texture: " << path << std::endl;
            }
        }
    }
    // if (!_defaultTexture.loadFromFile("GUI/assets/level1/front.png")) {
    //      std::cerr << "Failed to load default texture" << std::endl;
    // }
}

void Map::updatePlayerLevel(int id, int level)
{
    if (_players.find(id) != _players.end()) {
        _players[id]->setLevel(level);
    }
}

sf::Texture& Map::getPlayerTexture(int level, PlayerOrientation orientation)
{
    if (_playerTextures.count(level) && _playerTextures.at(level).count(orientation)) {
        return *(_playerTextures.at(level).at(orientation));
    }
    return _defaultTexture;
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
            for (int i = 0; i < 4; ++i) {
                points[i] = center + (points[i] - center) * scale;
            }
            sf::ConvexShape cell;
            cell.setPointCount(4);
            for (int i = 0; i < 4; ++i)
                cell.setPoint(i, points[i]);
            cell.setPosition(iso_x, iso_y);
            cell.setTexture(&textures[0]);
            cell.setOutlineThickness(1);
            cell.setOutlineColor(sf::Color::Black);
            window.draw(cell);
            for (size_t i = 0; i < buildingPositions.size(); ++i) {
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
