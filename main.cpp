#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

#include "menu.h"
#include "Kazik.h" 
#include "About.h"
#include "SlotMachines.h"

int main() {

	sf::RenderWindow window(sf::VideoMode(1280, 720), "The Kazik");

	// Загрузка шрифта
	sf::Font font;
	if (!font.loadFromFile("Fonts/firstFont.ttf")) return 1;


	// Иконка игры

	sf::Image icon;
	if (!icon.loadFromFile("Image/icon.png")) return 1;


	window.setIcon(32, 32, icon.getPixelsPtr());

	// Создание меню и значение баланса

	int balance = 1000;

	Menu menu(font);
	Balance balanceLabel(font, balance);

	// Панель игровая

	sf::Texture TexturInfoPanel;
	TexturInfoPanel.loadFromFile("Image/Panel.png");

	sf::RectangleShape GameInfoPanel(sf::Vector2f(1280, 113));
	GameInfoPanel.setTexture(&TexturInfoPanel);
	GameInfoPanel.setPosition(sf::Vector2f(0, 0));

	// Игровое меню

	sf::Texture textureBoard;
	textureBoard.loadFromFile("Image/Board.png");

	sf::RectangleShape GameBackground(sf::Vector2f(1280, 720));
	GameBackground.setTexture(&textureBoard);

	sf::RectangleShape GameBackground2(sf::Vector2f(1280, 720));
	GameBackground2.setTexture(&textureBoard);
	GameBackground2.setPosition(sf::Vector2f(1280, 0));

	// Табло игрока 

	sf::Texture DoomFace;

	sf::RectangleShape DoomFaceBackground(sf::Vector2f(302, 302));
	DoomFaceBackground.setPosition(sf::Vector2f(978, 418));

	// Состояния игры

	GameState currentState = MENU;

	// Игровой цикл
	sf::Vector2f pos;
	sf::Clock clock;
	float time;

	while (window.isOpen()) {
		if (balance <= 0) break;
		sf::Event event;
		time = clock.getElapsedTime().asMicroseconds();
		time /= 6000;
		clock.restart();

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		// Отрисовка


		window.draw(GameBackground);
		window.draw(GameBackground2);
		window.draw(GameInfoPanel);

		window.draw(DoomFaceBackground);
		balanceLabel.draw(window);

		// Обработка состояний игры
		if (currentState == MENU) {
			if (balance < 300) DoomFace.loadFromFile("Image/angrySmile.png");
			else if (balance > 10000) DoomFace.loadFromFile("Image/alegSmile.png");
			else DoomFace.loadFromFile("Image/normalSmile.png");


			int menuResult = menu.handleInput(window);

			if (menuResult == 1) { // Игровые автоматы
				currentState = SLOT_MACHINES;
			}
			else if (menuResult == 2) { // Рулетка
				currentState = ROULETTE;
			}
			else if (menuResult == 3) { // Настройки
				currentState = SETTINGS;
			}
			else if (menuResult == 4) { // О игре и авторе
				currentState = ABOUT;
			}
			else if (menuResult == 5) { // Выход
				currentState = EXIT;
			}

			menu.draw(window);

		}
		else if (currentState == SLOT_MACHINES) {
			static bool inputCompleted = false;
			static bool keyPressed = false;
			static std::string inputBet;
			static sf::Text inputBetText;
			static UserInput userInput(font, currentState);
			Button Back(sf::Vector2f(100, 200), sf::Vector2f(200, 50), "Back", font);
			Button Next(sf::Vector2f(100, 480), sf::Vector2f(200, 50), "Next", font);

			// Инициализация текстовых полей при первом входе в состояние
			static bool initialized = false;
			if (!initialized) {
				inputBetText.setFont(font);
				inputBetText.setCharacterSize(24);
				inputBetText.setFillColor(sf::Color::White);
				inputBetText.setPosition(1000, 140);


				inputCompleted = false;
				inputBet = "";
				initialized = true;
			}

			if (event.type != sf::Event::TextEntered) keyPressed = false;

			// Обработка ввода в реальном времени
			if (!inputCompleted) {
				// Обработка текстового ввода для ставки
				if (event.type == sf::Event::TextEntered && inputState == Bet && !keyPressed) {
					keyPressed = true;
					if (event.text.unicode == 8) { // Backspace
						if (!inputBet.empty()) inputBet.pop_back();
					}
					else if (event.text.unicode == 13) { // Enter - переход к вводу числа
						// Пустая обработка для Enter
					}
					else if (event.text.unicode < 128 && event.text.unicode != 13) {
						inputBet.push_back(event.text.unicode);

					}
					inputBetText.setString(inputBet);
				}

				// Крутим барабаны

				if (inputState == Done) { // Enter - завершение ввода
					sf::Texture textureMachine;
					textureMachine.loadFromFile("Image/SlotMachines.png");

					sf::RectangleShape MachineBackground(sf::Vector2f(1280, 720));
					MachineBackground.setTexture(&textureMachine);
					MachineBackground.setPosition(sf::Vector2f(0, 120));
					inputCompleted = true;

					Slots slot1, slot2, slot3;

					// Выполнение игры в рулетку
					int Bet = std::stoi(inputBet);

					slot1 = randomSlot();
					slot2 = randomSlot();
					slot3 = randomSlot();

					int Schrodinger = slotsCompare(slot1, slot2, slot3, Bet); // Ну типа тут как кот Шредингера: вроде и победил или проиграл
					if (Schrodinger) DoomFace.loadFromFile("Image/happySmile.png");
					else DoomFace.loadFromFile("Image/sadSmile.png");

					balance += Schrodinger;
					// Обновление баланса
					balanceLabel = Balance(font, balance);

					window.draw(MachineBackground);
				}

				// Отрисовка текущего состояния ввода
				userInput.draw(window);
				window.draw(inputBetText);
			}

			// После завершения ввода - кнопка для возврата в меню
			if (Back.isMouseOver(window)) {
				Back.setColor(sf::Color(100, 100, 100));
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

					currentState = MENU;
					inputCompleted = false;
					initialized = false;
					inputBet.clear();
				}
			}
			else {
				Back.setColor(sf::Color(70, 70, 70));
			}

			if (Next.isMouseOver(window)) {
				Next.setColor(sf::Color(100, 100, 100));
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

					inputState = Done;

				}
			}

			Back.draw(window);
			Next.draw(window);
		}
		else if (currentState == ROULETTE) {
			static bool inputCompleted = false;
			static bool keyPressed = false;
			static std::string inputBet, inputNumber;
			static sf::Text inputBetText, inputNumberText;
			static UserInput userInput(font, currentState);
			Button Back(sf::Vector2f(100, 200), sf::Vector2f(200, 50), "Back", font);
			Button Next(sf::Vector2f(100, 480), sf::Vector2f(200, 50), "Next", font);

			// Инициализация текстовых полей при первом входе в состояние
			static bool initialized = false;
			if (!initialized) {
				inputBetText.setFont(font);
				inputBetText.setCharacterSize(24);
				inputBetText.setFillColor(sf::Color::White);
				inputBetText.setPosition(1000, 140);

				inputNumberText.setFont(font);
				inputNumberText.setCharacterSize(24);
				inputNumberText.setFillColor(sf::Color::White);
				inputNumberText.setPosition(1000, 200);

				inputCompleted = false;
				inputBet = "";
				inputNumber = "";
				initialized = true;
			}

			if (event.type != sf::Event::TextEntered) keyPressed = false;

			// Обработка ввода в реальном времени
			if (!inputCompleted) {
				// Обработка текстового ввода для ставки
				if (event.type == sf::Event::TextEntered && inputState == Bet && !keyPressed) {
					keyPressed = true;
					if (event.text.unicode == 8) { // Backspace
						if (!inputBet.empty()) inputBet.pop_back();
					}
					else if (event.text.unicode == 13) { // Enter - переход к вводу числа
						// Пустая обработка для Enter
					}
					else if (event.text.unicode < 128 && event.text.unicode != 13) {
						inputBet.push_back(event.text.unicode);

					}
					inputBetText.setString(inputBet);
				}

				// Обработка текстового ввода для числа (только после ввода ставки)
				if (inputState == Number && event.type == sf::Event::TextEntered && !keyPressed) {
					keyPressed = true;
					if (event.text.unicode == 8) { // Backspace
						if (!inputNumber.empty()) inputNumber.pop_back();
					}
					else if (event.text.unicode == 13) { // Enter - завершение ввода
						if (!inputNumber.empty()) {
							inputCompleted = true;

							// Выполнение игры в рулетку
							int Bet = std::stoi(inputBet);
							int num = std::stoi(inputNumber);
							int random_number = rand() % 9 + 1;


							if (random_number == num) {
								balance += Bet;
								DoomFace.loadFromFile("Image/happySmile.png");
							}
							else {
								balance -= Bet;
								DoomFace.loadFromFile("Image/sadSmile.png");
							}

							// Обновление баланса
							balanceLabel = Balance(font, balance);
						}
					}
					else if (event.text.unicode < 128 && event.text.unicode != 13) {
						inputNumber.push_back(event.text.unicode);
					}
					inputNumberText.setString(inputNumber);
				}

				// Отрисовка текущего состояния ввода
				userInput.draw(window);
				window.draw(inputBetText);
				window.draw(inputNumberText);
			}

			// После завершения ввода - кнопка для возврата в меню
			if (Back.isMouseOver(window)) {
				Back.setColor(sf::Color(100, 100, 100));
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

					currentState = MENU;
					inputCompleted = false;
					initialized = false;
					inputBet.clear();
					inputNumber.clear();
				}
			}
			else {
				Back.setColor(sf::Color(70, 70, 70));
			}

			if (Next.isMouseOver(window)) {
				Next.setColor(sf::Color(100, 100, 100));
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

					inputState = Number;

				}
			}
			else {
				Next.setColor(sf::Color(70, 70, 70));
			}
			userInput.draw(window);

			Back.draw(window);
			Next.draw(window);
		}

		else if (currentState == SETTINGS) {
			// Емае че в настройках делать
			currentState = MENU;
		}
		else if (currentState == ABOUT) {
			About about(font);
			DoomFace.loadFromFile("Image/wtfSmile.png");
			int aboutResult = about.backInput(window);

			if (aboutResult == true) {
				currentState = MENU;
			}

			about.draw(window);
		}
		else { // currentState == EXIT
			break;
		}

		DoomFaceBackground.setTexture(&DoomFace);

		// Движение фона

		GameBackground.move(-0.2 * time, 0);
		pos = GameBackground.getPosition();
		if (pos.x < -1280) GameBackground.setPosition(1280, pos.y);
		GameBackground2.move(-0.2 * time, 0);
		pos = GameBackground2.getPosition();
		if (pos.x < -1280) GameBackground2.setPosition(1280, pos.y);

		window.display();

	}
	window.close();
	return 0;
}