## The Kazik - Casino Game
A C++ casino-style game built with SFML featuring slot machines and roulette.

## Description
The Kazik is a graphical casino game where players can test their luck with various gambling games. The project features a polished UI with animated backgrounds, interactive elements, and multiple game modes.

## Features
Main Menu: Navigate between different game modes and options

Slot Machines: Classic slot machine with visual reels and random outcomes

Roulette: Number guessing game (0-10) with betting system

Dynamic UI: Animated backgrounds and responsive interface

Balance System: Track your virtual money across game sessions

Visual Feedback: Changing character expressions based on game outcomes and balance

## Requirements
### System Requirements
C++17 compatible compiler

SFML 2.6.2 or later

Windows/Linux/macOS

### Dependencies
SFML Graphics module

SFML Window module

SFML System module

## Build the Project

### bash
g++ -c main.cpp -I/path/to/sfml/include
g++ main.o -o the_kazik -L/path/to/sfml/lib -lsfml-graphics -lsfml-window -lsfml-system

## Game Rules
Start with 1000 credits

Game ends if balance reaches 0

Character face changes based on your balance and wins/losses

Slot machines have different symbol combinations with varying payouts

Roulette pays 1:1 if you guess correctly

## Controls
Mouse: Navigate menus and click buttons

Keyboard: Enter bet amounts and numbers

Backspace: Delete input characters

## Key Classes
Menu - Handles main menu navigation

Balance - Displays and manages player balance

About - Shows project information

UserInput - Handles text input for bets

Button - Interactive UI buttons

## Development
Compiling from Source
Make sure SFML libraries are properly linked in your build system. The project uses standard C++17 features.

Adding New Games
Add new game state to GameState enum

Implement game logic in main loop under new state condition

Add navigation from main menu

License
This project is for educational purposes. Please ensure you comply with SFML's licensing terms and any asset licenses.

## Notes
This is a simulation - no real money is involved

Game uses pseudo-random number generation

All assets should be placed in correct directories for proper functionality

Balance resets to 1000 each time the game starts

Enjoy playing The Kazik! 🎰
