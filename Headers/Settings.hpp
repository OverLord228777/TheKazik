#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "Buttons.hpp"
#include "musik.hpp"

class Settings {
public:
    Settings(sf::Font& font) 
        : buttonBack(sf::Vector2f(100, 600), sf::Vector2f(200, 50), "Back", font),
        buttonSave(sf::Vector2f(100, 550), sf::Vector2f(200, 50), "Save updates", font), 
        buttonNextMusic(sf::Vector2f(100, 120), sf::Vector2f(200, 50), "Check a next music", font), 
        buttonUpMusic(sf::Vector2f(100, 200), sf::Vector2f(50, 50), "+", font),
        buttonDownMusic(sf::Vector2f(400, 200), sf::Vector2f(50, 50), "-", font) {

        musicName.setFont(font);  
        musicName.setCharacterSize(24); 
        musicName.setFillColor(sf::Color::Blue);
        musicName.setPosition(400, 200);

        soundVolume.setFont(font);
        soundVolume.setCharacterSize(24);
        soundVolume.setFillColor(sf::Color::Blue);
        soundVolume.setPosition(400, 200);
    }

    void setMusicName(std::string text) {
        musicName.setString(text);
    }

    void setSoundVolume(float vol, KazikMusic music) {
        std::string text = std::to_string(vol);
        musicName.setString(text);

        music.setVolume(vol);
    }

    void draw(sf::RenderWindow& window) {
        buttonSave.draw(window);
        buttonBack.draw(window);
        buttonNextMusic.draw(window);
        buttonDownMusic.draw(window);
        buttonUpMusic.draw(window);
        window.draw(soundVolume);
        window.draw(musicName);
    }

    bool changeVolumeUpInput(sf::RenderWindow& window) {
        if (buttonUpMusic.isMouseOver(window)) {
            buttonUpMusic.setColor(sf::Color(100, 100, 100));
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                return true;
            }
        } 
        else {
            buttonUpMusic.setColor(sf::Color(70, 70, 70));
        }

        return false;
    }

    bool changeVolumeDownInput(sf::RenderWindow& window) {
        if (buttonDownMusic.isMouseOver(window)) {
            buttonDownMusic.setColor(sf::Color(100, 100, 100));
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                return true;
            }
        }
        else {
            buttonDownMusic.setColor(sf::Color(70, 70, 70));
        }

        return false;
    }

    bool nextMusicInput(sf::RenderWindow& window) {
        if (buttonNextMusic.isMouseOver(window)) {
            buttonNextMusic.setColor(sf::Color(100, 100, 100));
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                return true;
            }
        } // I can unite these functions about button Input but i don't know how and im lazy
        else {
            buttonNextMusic.setColor(sf::Color(70, 70, 70));
        }

        return false;
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
    sf::Text musicName;
    sf::Text soundVolume;
    Button buttonBack;
    Button buttonSave;
    Button buttonNextMusic;
    Button buttonUpMusic;
    Button buttonDownMusic;
};