#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "Buttons.h"

class Menu {
public:
    Menu(sf::Font& font) {
        std::vector<std::string> buttonNames = { "Slot Machines", "Roulette", "Settings", "About", "Exit" };
        float startY = 250;

        for (int i = 0; i < buttonNames.size(); ++i) {
            buttons.push_back(Button(sf::Vector2f(540, startY + i * 80), sf::Vector2f(200, 50), buttonNames[i], font));
        }

        title.setFont(font);
        title.setString("WELCOME TO THE KAZIK!");
        title.setCharacterSize(60);
        title.setFillColor(sf::Color::Black);
        title.setStyle(sf::Text::Bold);

        sf::FloatRect titleRect = title.getLocalBounds();
        title.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
        title.setPosition(640, 150);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(title);
        for (auto& button : buttons) {
            button.draw(window);
        }
    }

    int handleInput(sf::RenderWindow& window) {
        for (int i = 0; i < buttons.size(); ++i) {
            if (buttons[i].isMouseOver(window)) {
                buttons[i].setColor(sf::Color(100, 100, 100));
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    return i + 1; // Возвращаем номер кнопки (1-4)
                }
            }
            else {
                buttons[i].setColor(sf::Color(70, 70, 70));
            }
        }
        return 0;
    }

private:
    std::vector<Button> buttons;
    sf::Text title;
};