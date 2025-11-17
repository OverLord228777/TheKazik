#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Button {
public:
    Button(sf::Vector2f position, sf::Vector2f size, std::string text, sf::Font& font) {
        shape.setSize(size);
        shape.setPosition(position);
        shape.setFillColor(sf::Color(70, 70, 70));
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::Black);

        this->text.setFont(font);
        this->text.setString(text);
        this->text.setCharacterSize(24);
        this->text.setFillColor(sf::Color::White);

        // Центрирование текста в кнопке
        sf::FloatRect textRect = this->text.getLocalBounds();
        this->text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        this->text.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }

    bool isMouseOver(sf::RenderWindow& window) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    }

    void setColor(sf::Color color) {
        shape.setFillColor(color);
    }

private:
    sf::RectangleShape shape;
    sf::Text text;
};