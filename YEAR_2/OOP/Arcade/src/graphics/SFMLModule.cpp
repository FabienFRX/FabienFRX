/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Mar, 2025                                                     *
 * Title           - B-OOP-400-TLS-4-1-arcade-nolan.dumoulin [WSL: Debian]              *
 * Description     -                                                                    *
 *     SFMLModule                                                                       *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _|           *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _|           *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_|           *
 *       _|        _|          _|        _|      _|        _|        _|    _|           *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _|           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "SFMLModule.hpp"

namespace arcade {
    SFMLModule::SFMLModule() : ADisplayModule("SFML", 800, 600), window(nullptr), font(nullptr), resourceManager(new ResourceManager()) {}

    SFMLModule::~SFMLModule()
    {
        stop();
    }

    void SFMLModule::init()
    {
        window = new sf::RenderWindow(sf::VideoMode(width, height), "Arcade - SFML", sf::Style::Titlebar | sf::Style::Close);
        if (!window) {
            throw DisplayException("Failed to create SFML window");
        }

        window->setFramerateLimit(60);

        font = new sf::Font();

        const std::vector<std::string> fontPaths = {
            "./assets/fonts/DejaVuSansCondensed.ttf",
            "assets/fonts/DejaVuSansCondensed.ttf",
            "/usr/share/fonts/truetype/dejavu/DejaVuSansCondensed.ttf",
            "/usr/share/fonts/TTF/DejaVuSans.ttf",
            "/usr/share/fonts/dejavu/DejaVuSans.ttf"
        };

        bool fontLoaded = false;
        for (const auto& path : fontPaths) {
            if (std::filesystem::exists(path) && font->loadFromFile(path)) {
                fontLoaded = true;
                break;
            }
        }

        if (!fontLoaded) {
            delete font;
            font = nullptr;
            std::cerr << "Warning: Could not load any font, text rendering will be disabled" << std::endl;
        }

        open = true;
    }

    void SFMLModule::stop()
    {
        open = false;

        if (resourceManager) {
            delete resourceManager;
            resourceManager = nullptr;
        }

        if (font) {
            delete font;
            font = nullptr;
        }

        if (window) {
            window->close();
            delete window;
            window = nullptr;
        }
    }

    void SFMLModule::clear()
    {
        if (window) {
            window->clear(sf::Color::Black);
        }
    }

    void SFMLModule::display()
    {
        if (window) {
            window->display();
        }
    }

    bool SFMLModule::pollEvent(InputEvent &event)
    {
        if (!window) {
            return false;
        }

        sf::Event sfEvent;
        if (window->pollEvent(sfEvent)) {
            switch (sfEvent.type) {
                case sf::Event::Closed:
                    event.type = EventType::WindowClosed;
                    return true;

                case sf::Event::KeyPressed:
                    event.type = EventType::KeyPressed;

                    switch (sfEvent.key.code) {
                        case sf::Keyboard::Up:
                            event.key.keyCode = KeyCode::Up;
                            break;
                        case sf::Keyboard::Down:
                            event.key.keyCode = KeyCode::Down;
                            break;
                        case sf::Keyboard::Left:
                            event.key.keyCode = KeyCode::Left;
                            break;
                        case sf::Keyboard::Right:
                            event.key.keyCode = KeyCode::Right;
                            break;
                        case sf::Keyboard::Return:
                            event.key.keyCode = KeyCode::Enter;
                            break;
                        case sf::Keyboard::Escape:
                            event.key.keyCode = KeyCode::Escape;
                            break;
                        case sf::Keyboard::Space:
                            event.key.keyCode = KeyCode::Space;
                            break;
                        case sf::Keyboard::BackSpace:
                            event.key.keyCode = KeyCode::Backspace;
                            break;
                        case sf::Keyboard::Tab:
                            event.key.keyCode = KeyCode::Tab;
                            break;
                        case sf::Keyboard::LShift:
                            event.key.keyCode = KeyCode::Shift;
                            break;
                        default:
                            if (sfEvent.key.code >= sf::Keyboard::A && sfEvent.key.code <= sf::Keyboard::Z) {
                                event.key.keyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::A) + (sfEvent.key.code - sf::Keyboard::A));
                            } else if (sfEvent.key.code >= sf::Keyboard::Num0 && sfEvent.key.code <= sf::Keyboard::Num9) {
                                event.key.keyCode = static_cast<KeyCode>(static_cast<int>(KeyCode::Num0) + (sfEvent.key.code - sf::Keyboard::Num0));
                            } else {
                                event.key.keyCode = KeyCode::Unknown;
                            }
                            break;
                    }
                    return true;

                case sf::Event::MouseButtonPressed:
                    event.type = EventType::MouseButtonPressed;
                    event.mouseButton.x = sfEvent.mouseButton.x;
                    event.mouseButton.y = sfEvent.mouseButton.y;

                    switch (sfEvent.mouseButton.button) {
                        case sf::Mouse::Left:
                            event.mouseButton.button = 0;
                            break;
                        case sf::Mouse::Right:
                            event.mouseButton.button = 1;
                            break;
                        case sf::Mouse::Middle:
                            event.mouseButton.button = 2;
                            break;
                        default:
                            event.mouseButton.button = 0;
                            break;
                    }
                    return true;

                case sf::Event::MouseMoved:
                    event.type = EventType::MouseMoved;
                    event.mouseMove.x = sfEvent.mouseMove.x;
                    event.mouseMove.y = sfEvent.mouseMove.y;
                    return true;

                default:
                    break;
            }
        }

        return false;
    }

    sf::Color SFMLModule::convertColor(const Color &color)
    {
        return sf::Color(color.r, color.g, color.b, color.a);
    }

    void SFMLModule::drawRect(int x, int y, int width, int height, const Color &color)
    {
        if (!window) {
            return;
        }

        sf::RectangleShape rect(sf::Vector2f(width, height));
        rect.setPosition(x, y);
        rect.setFillColor(convertColor(color));

        window->draw(rect);
    }

    void SFMLModule::drawCircle(int x, int y, int radius, const Color &color)
    {
        if (!window) {
            return;
        }

        sf::CircleShape circle(radius);
        circle.setPosition(x - radius, y - radius);
        circle.setFillColor(convertColor(color));

        window->draw(circle);
    }

    void SFMLModule::drawText(int x, int y, const std::string &text, const Color &color)
    {
        if (!window || !font) {
            if (window) {
                sf::RectangleShape rect(sf::Vector2f(text.length() * 10, 20));
                rect.setPosition(x, y);
                rect.setFillColor(sf::Color(50, 50, 50));
                window->draw(rect);
            }
            return;
        }

        sf::Text sfText;
        sfText.setFont(*font);
        sfText.setString(text);
        sfText.setCharacterSize(18);
        sfText.setFillColor(convertColor(color));
        sfText.setPosition(x, y);

        window->draw(sfText);
    }

    bool SFMLModule::isOpen() const
    {
        return open && window && window->isOpen();
    }

    void SFMLModule::drawLine(int x1, int y1, int x2, int y2, const Color &color)
    {
        if (!window) {
            return;
        }

        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x1, y1), convertColor(color)),
            sf::Vertex(sf::Vector2f(x2, y2), convertColor(color))
        };

        window->draw(line, 2, sf::Lines);
    }

    void SFMLModule::drawRectOutline(int x, int y, int width, int height, const Color &color, int outlineThickness)
    {
        if (!window) {
            return;
        }

        sf::RectangleShape rect(sf::Vector2f(width, height));
        rect.setPosition(x, y);
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(convertColor(color));
        rect.setOutlineThickness(outlineThickness);

        window->draw(rect);
    }

    void SFMLModule::drawCircleOutline(int x, int y, int radius, const Color &color, int outlineThickness)
    {
        if (!window) {
            return;
        }

        sf::CircleShape circle(radius);
        circle.setPosition(x - radius, y - radius);
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineColor(convertColor(color));
        circle.setOutlineThickness(outlineThickness);

        window->draw(circle);
    }

    void SFMLModule::drawSprite(int x, int y, const std::string &texturePath, int width, int height)
    {
        if (!window) {
            return;
        }

        sf::Texture* texture = resourceManager->getResource<sf::Texture>(texturePath);

        if (!texture) {
            texture = new sf::Texture();
            if (texture->loadFromFile(texturePath)) {
                resourceManager->addResource<sf::Texture>(texturePath, texture);
            } else {
                delete texture;
                return;
            }
        }

        sf::Sprite sprite(*texture);
        sprite.setPosition(x, y);

        if (width > 0 && height > 0) {
            float scaleX = static_cast<float>(width) / texture->getSize().x;
            float scaleY = static_cast<float>(height) / texture->getSize().y;
            sprite.setScale(scaleX, scaleY);
        }

        window->draw(sprite);
    }

    extern "C" {
        IDisplayModule *createDisplayModule() {
            return new SFMLModule();
        }
    }
}