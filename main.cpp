#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <array>
#include <algorithm>
#include <cctype>
#include <exception>

#include "Headers/menu.hpp"
#include "Headers/Kazik.hpp" 
#include "Headers/About.hpp"
#include "Headers/SlotMachines.hpp"
#include "Headers/musik.hpp"
#include "Headers/Settings.hpp"

bool tryParseUnsignedInt(const std::string& value, int& result) {
	if (value.empty()) return false;
	if (!std::all_of(value.begin(), value.end(), [](unsigned char ch) { return std::isdigit(ch); })) {
		return false;
	}
	try {
		result = std::stoi(value);
	}
	catch (const std::exception&) {
		return false;
	}
	return true;
}

int main() {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	sf::RenderWindow window(sf::VideoMode(1280, 720), "The Kazik");
	window.setFramerateLimit(60);

	// Load primary UI font
	sf::Font font;
	if (!font.loadFromFile("Fonts/firstFont.ttf")) return 1;


	// Load window icon

	sf::Image icon;
	if (!icon.loadFromFile("Image/icon.png")) return 1;

	window.setIcon(32, 32, icon.getPixelsPtr());

	// Initialize player balance and HUD labels

	int balance = 1000;

	Menu menu(font);
	Balance balanceLabel(font, balance);

	// Load info panel texture

	sf::Texture TexturInfoPanel;
	TexturInfoPanel.loadFromFile("Image/Panel.png");

	sf::RectangleShape GameInfoPanel(sf::Vector2f(1280, 113));
	GameInfoPanel.setTexture(&TexturInfoPanel);
	GameInfoPanel.setPosition(sf::Vector2f(0, 0));

	// Prepare scrolling board background

	sf::Texture textureBoard;
	textureBoard.loadFromFile("Image/Board.png");

	sf::RectangleShape GameBackground(sf::Vector2f(1280, 720));
	GameBackground.setTexture(&textureBoard);

	sf::RectangleShape GameBackground2(sf::Vector2f(1280, 720));
	GameBackground2.setTexture(&textureBoard);
	GameBackground2.setPosition(sf::Vector2f(1280, 0));

	// Configure Doom face portrait slot

	sf::Texture DoomFace;

	sf::RectangleShape DoomFaceBackground(sf::Vector2f(302, 302));
	DoomFaceBackground.setPosition(sf::Vector2f(978, 418));

	// Track current high-level game state

	GameState currentState = MENU;

	// Background animation helpers
	sf::Vector2f pos;
	sf::Clock clock;
	float time;

	// Music 

	KazikMusic music;
	float volume = 50.f;
	std::string musicName = "Govno";
	if (!music.load("Music/Govno.ogg")) {
		return -1;
	}
	music.play();

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

		// Draw static background layers

		window.draw(GameBackground);
		window.draw(GameBackground2);
		window.draw(GameInfoPanel);

		balanceLabel.draw(window);

		// Handle what to show based on current state
		if (currentState == MENU) {
			if (balance < 300) DoomFace.loadFromFile("Image/angrySmile.png");
			else if (balance > 10000) DoomFace.loadFromFile("Image/alegSmile.png");
			else DoomFace.loadFromFile("Image/normalSmile.png");


			int menuResult = menu.handleInput(window);

			if (menuResult == 1) { // Slot machines
				currentState = SLOT_MACHINES;
			}
			else if (menuResult == 2) { // Roulette
				currentState = ROULETTE;
			}
			else if (menuResult == 3) { // Settings
				currentState = SETTINGS;
			}
			else if (menuResult == 4) { // About / credits
				currentState = ABOUT;
			}
			else if (menuResult == 5) { // Exit
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
			static sf::Texture textureMachine;
			static sf::RectangleShape MachineBackground;
			static std::array<sf::Texture, 3> slotTextures;
			static std::array<sf::RectangleShape, 3> slotShapes;
			static bool slotResultReady = false;
			static sf::Text betValidationMessage;

			// One-time setup for slot machine input and visuals
			static bool initialized = false;
			if (!initialized) {
				inputBetText.setFont(font);
				inputBetText.setCharacterSize(24);
				inputBetText.setFillColor(sf::Color::Blue);
				inputBetText.setPosition(1000, 140);


				inputCompleted = false;
				inputBet = "";
				slotResultReady = false;

				MachineBackground.setSize(sf::Vector2f(1280.f, 720.f));
				MachineBackground.setPosition(sf::Vector2f(0.f, 0.f));

				slotShapes[0].setSize(sf::Vector2f(150.f, 185.f));
				slotShapes[1].setSize(sf::Vector2f(150.f, 185.f));
				slotShapes[2].setSize(sf::Vector2f(150.f, 185.f));

				slotShapes[0].setPosition(sf::Vector2f(340.f, 270.f));
				slotShapes[1].setPosition(sf::Vector2f(531.f, 270.f));
				slotShapes[2].setPosition(sf::Vector2f(723.f, 273.f));

				betValidationMessage.setFont(font);
				betValidationMessage.setCharacterSize(20);
				betValidationMessage.setFillColor(sf::Color::Red);
				betValidationMessage.setPosition(1000.f, 110.f);
				betValidationMessage.setString("");
				initialized = true;
			}

			if (event.type != sf::Event::TextEntered) keyPressed = false;

			// Handle input while in the slot machine state
			if (!inputCompleted) {
				// Capture bet amount characters
				if (event.type == sf::Event::TextEntered && inputState == Bet && !keyPressed) {
					keyPressed = true;
					if (event.text.unicode == 8) { // Backspace
						if (!inputBet.empty()) inputBet.pop_back();
					}
					else if (event.text.unicode < 128 && event.text.unicode != 13) {
						inputBet.push_back(event.text.unicode);

					}
					inputBetText.setString(inputBet);
					betValidationMessage.setString("");
					slotResultReady = false;
				}

				// Validate bet and spin reels

				if (inputState == Done) {
					int Bet = 0;
					if (!tryParseUnsignedInt(inputBet, Bet) || Bet <= 0) {
						betValidationMessage.setString("Enter a positive numeric bet");
						// slotResultReady = false;
					}
					else if (!textureMachine.loadFromFile("Image/SlotMachines.png")) {
						betValidationMessage.setString("SlotMachines.png missing");
						slotResultReady = false;
					}
					else {
						MachineBackground.setTexture(&textureMachine);

						Slots slot1 = randomSlot();
						Slots slot2 = randomSlot();
						Slots slot3 = randomSlot();

						slotTextures[0].loadFromFile(getSlotPNG(slot1));
						slotTextures[1].loadFromFile(getSlotPNG(slot2));
						slotTextures[2].loadFromFile(getSlotPNG(slot3));

						slotShapes[0].setTexture(&slotTextures[0]);
						slotShapes[1].setTexture(&slotTextures[1]);
						slotShapes[2].setTexture(&slotTextures[2]);

						bool win = true;
						int Schrodinger = slotsCompare(slot1, slot2, slot3, Bet, &win); // Determine payout; positive means win, negative means loss
						if (win) DoomFace.loadFromFile("Image/happySmile.png");
						else DoomFace.loadFromFile("Image/sadSmile.png");

						balance += Schrodinger;
						// Refresh balance label after the result
						balanceLabel = Balance(font, balance);
						betValidationMessage.setString("");
						slotResultReady = true;
						inputBet.clear();
						inputBetText.setString("");
					}
					inputState = InputState::Bet;
				}

				// Draw slot input controls
				userInput.draw(window);
				window.draw(inputBetText);
				if (!betValidationMessage.getString().isEmpty()) {
					window.draw(betValidationMessage);
				}
			}

			if (slotResultReady) {
				window.draw(MachineBackground);
				for (const auto& slotShape : slotShapes) {
					window.draw(slotShape);
				}
			}

			// Back button hover/press handling
			if (Back.isMouseOver(window)) {
				Back.setColor(sf::Color(100, 100, 100));
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

					currentState = MENU;
					inputCompleted = false;
					initialized = false;
					inputBet.clear();
					slotResultReady = false;
					betValidationMessage.setString("");
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
			static sf::Text rouletteValidationMessage;
			Button Back(sf::Vector2f(100, 200), sf::Vector2f(200, 50), "Back", font);
			Button Next(sf::Vector2f(100, 480), sf::Vector2f(200, 50), "Next", font);

			// One-time setup for roulette input and visuals
			static bool initialized = false;
			if (!initialized) {
				inputBetText.setFont(font);
				inputBetText.setCharacterSize(24);
				inputBetText.setFillColor(sf::Color::Blue);
				inputBetText.setPosition(1000, 140);

				inputNumberText.setFont(font);
				inputNumberText.setCharacterSize(24);
				inputNumberText.setFillColor(sf::Color::Blue);
				inputNumberText.setPosition(1000, 200);

				inputCompleted = false;
				inputBet = "";
				inputNumber = "";
				rouletteValidationMessage.setFont(font);
				rouletteValidationMessage.setCharacterSize(20);
				rouletteValidationMessage.setFillColor(sf::Color::Red);
				rouletteValidationMessage.setPosition(1000.f, 100.f);
				rouletteValidationMessage.setString("");
				initialized = true;
			}

			if (event.type != sf::Event::TextEntered) keyPressed = false;

			// Handle input while in the roulette state
			if (!inputCompleted) {
				// Capture roulette bet characters
				if (event.type == sf::Event::TextEntered && inputState == Bet && !keyPressed) {
					keyPressed = true;
					if (event.text.unicode == 8) { // Backspace
						if (!inputBet.empty()) inputBet.pop_back();
					}
					else if (event.text.unicode == 13) { // Enter - advance to number input
						// Reserved hook for Enter-specific handling
					}
					else if (event.text.unicode < 128 && event.text.unicode != 13) {
						inputBet.push_back(event.text.unicode);

					}
					inputBetText.setString(inputBet);
					rouletteValidationMessage.setString("");
				}

				// Capture roulette number characters (after bet entered)
				if (inputState == Number && event.type == sf::Event::TextEntered && !keyPressed) {
					keyPressed = true;
					if (event.text.unicode == 8) { // Backspace
						if (!inputNumber.empty()) inputNumber.pop_back();
					}
					else if (event.text.unicode == 13) { // Enter - finalize roulette guess
						if (!inputNumber.empty()) {
							inputCompleted = true;

							// Validate bet and chosen number
							int Bet = 0;
							int num = 0;
							bool betValid = tryParseUnsignedInt(inputBet, Bet) && Bet > 0;
							bool numberValid = tryParseUnsignedInt(inputNumber, num) && num >= 0 && num <= 10;

							if (!betValid) {
								rouletteValidationMessage.setString("Enter a positive numeric bet");
								inputCompleted = false;
							}
							else if (!numberValid) {
								rouletteValidationMessage.setString("Number must be 0-10");
								inputCompleted = false;
							}
							else {
								int random_number = rand() % 9 + 1;

								if (random_number == num) {
									balance += Bet;
									DoomFace.loadFromFile("Image/happySmile.png");
								}
								else {
									balance -= Bet;
									DoomFace.loadFromFile("Image/sadSmile.png");
								}

								rouletteValidationMessage.setString("");

								// Refresh balance label after roulette result
								balanceLabel = Balance(font, balance);
							}
						}
					}
					else if (event.text.unicode < 128 && event.text.unicode != 13) {
						inputNumber.push_back(event.text.unicode);
					}
					inputNumberText.setString(inputNumber);
					rouletteValidationMessage.setString("");
				}

				// Draw roulette input controls
				userInput.draw(window);
				window.draw(inputBetText);
				window.draw(inputNumberText);
				if (!rouletteValidationMessage.getString().isEmpty()) {
					window.draw(rouletteValidationMessage);
				}
			}

			// Back button hover/press handling
			if (Back.isMouseOver(window)) {
				Back.setColor(sf::Color(100, 100, 100));
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

					currentState = MENU;
					inputCompleted = false;
					initialized = false;
					inputBet.clear();
					inputNumber.clear();
					rouletteValidationMessage.setString("");
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
			Settings settings(font);
			bool keyPressed = false;
			int settingsResult = settings.backInput(window);
			int settingsNextMusik = settings.nextMusicInput(window);
			int settingsUpMusik = settings.changeVolumeUpInput(window);
			int settingsDownMusik = settings.changeVolumeDownInput(window);

			if (settingsResult == true) {
				currentState = MENU;
			}

			if (settingsUpMusik == true) {
				volume += 10;
				music.setVolume(volume);
			}

			if (settingsDownMusik == true) {
				volume -= 10;
				music.setVolume(volume);
			}

			// !!!!!!!
			if (settingsNextMusik == false) keyPressed = false;

			if (settingsNextMusik == true && !keyPressed) {
				keyPressed = true;
				music.stop();

				musicName = switch_next_track();
				if (musicName == "Govno") 
					music.load("Music/Govno.ogg");		
				else if (musicName == "Povorot ne tuda") 
					music.load("Music/YuraHoi.mp3");	
				else if (musicName == "Hava Nagila")
					music.load("Music/HavaNagila.mp3");
				else return -1;

				music.play();
			}
			settings.setMusicName(musicName);
			settings.draw(window);
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

		window.draw(DoomFaceBackground);
		DoomFaceBackground.setTexture(&DoomFace);

		// Animate the parallax background

		GameBackground.move(-0.2 * time, 0);
		pos = GameBackground.getPosition();
		if (pos.x < -1280) GameBackground.setPosition(1280, pos.y);
		GameBackground2.move(-0.2 * time, 0);
		pos = GameBackground2.getPosition();
		if (pos.x < -1280) GameBackground2.setPosition(1280, pos.y);

		window.display();

	}
	music.stop();
	window.close();
	return 0;
}