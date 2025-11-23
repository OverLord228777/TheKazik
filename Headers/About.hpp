#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "Buttons.hpp"

class About {
public:
    About(sf::Font& font) : buttonBack(sf::Vector2f(100, 120), sf::Vector2f(200, 50), "Back", font) {
        buttonBack = Button(sf::Vector2f(100, 120), sf::Vector2f(200, 50), "Back", font);

        aboutGame.setFont(font);
        aboutGame.setString(text);
        aboutGame.setCharacterSize(24); // Размер шрифта для лучшего отображения
        aboutGame.setFillColor(sf::Color::Blue);
        aboutGame.setPosition(100, 200); // Центрирование для многострочного текста

    }

    void draw(sf::RenderWindow& window) {
        window.draw(aboutGame);
        buttonBack.draw(window);
    }

    bool backInput(sf::RenderWindow& window) {
        if (buttonBack.isMouseOver(window)) {
            buttonBack.setColor(sf::Color(100, 100, 100));
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                return true;
            }
        }
        else {
            buttonBack.setColor(sf::Color(70, 70, 70));
        }

        return false;
    }

private:
    sf::Text aboutGame;
    std::string text = "The game was created on the initiative\nof the brilliant OverLord228777,\n"
        "at the request of his wonderful friend Andryusha,\n"
        "who doesn't know how to use Excel and VBA";
    Button buttonBack;
};