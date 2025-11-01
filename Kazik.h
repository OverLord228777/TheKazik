#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "Buttons.h"

enum GameState { MENU, SLOT_MACHINES, ROULETTE, SETTINGS, ABOUT, EXIT };

enum InputState { Bet, Number };
InputState inputState = Bet;

class Balance {
public:
    Balance(sf::Font& font, int balance) {
        std::string balanceStr = std::to_string(balance);

        titleBalance.setFont(font);
        titleBalance.setString(balanceStr + "$");
        titleBalance.setCharacterSize(50);
        titleBalance.setFillColor(sf::Color::White);

        titleBalance.setPosition(480, 20);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(titleBalance);      
    }

private:
    sf::Text titleBalance;
};

class UserInput {
public:
    UserInput(sf::Font& font, GameState gameState) { 
        //Next(sf::Vector2f(100, 180), sf::Vector2f(200, 50), "Next", font) {
        
        titleBet.setFont(font);
        titleBet.setString("Enter your bet: ");
        titleBet.setCharacterSize(50);
        titleBet.setFillColor(sf::Color::White);
        titleBet.setPosition(500, 140);

        if (gameState == ROULETTE) {
            titleNumber.setFont(font);
            titleNumber.setString("Enter your number (0-10): ");
            titleNumber.setCharacterSize(50);
            titleNumber.setFillColor(sf::Color::White);
            titleNumber.setPosition(500, 200);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(titleBet);
        window.draw(titleNumber);

        //Next.draw(window);
    }
    
    std::string textInput(sf::RenderWindow& window, sf::Font& font, sf::Event event, sf::Text inputText, int x, int y) {
        std::string userInput;

        inputText.setFont(font);
        inputText.setCharacterSize(24);
        inputText.setFillColor(sf::Color::White);
        inputText.setPosition(x, y);

        bool textEntered = false;
        while (!textEntered) {
            if (event.type == sf::Event::TextEntered)
            {
                // Игнорируем управляющие символы (Backspace, Enter и т.д.)
                if (event.text.unicode < 128) {
                    char enteredChar = static_cast<char>(event.text.unicode);

                    // Обработка Backspace
                    if (event.text.unicode == 8) {
                        if (!userInput.empty())
                            userInput.pop_back();
                    }
                    // Обычные символы
                    else if (enteredChar != '\r') { // Игнорируем Enter
                        userInput += enteredChar;
                    }

                    inputText.setString(userInput);
                }
            }
            window.draw(inputText);
            // Обработка нажатия Enter для завершения ввода
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    textEntered = true;

                }
            }
        }
        return userInput;
    }
private:
    sf::Text titleBet;
    sf::Text titleNumber;

    
    //
};