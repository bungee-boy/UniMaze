#include <SFML/Graphics.hpp>
#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include <thread>
#include <time.h>

#include "main.h"

using namespace std;

sf::Vector2i RandSpace() {  // Find random space
	sf::Vector2i Rand;
	while (true) {  // Search until space found
		Rand.x = rand() % (gMAZE_X - 2) + 1;  // Efficient rand (doesnt check outer walls)
		Rand.y = rand() % (gMAZE_Y - 2) + 1;
		if (gMap.currLevel[Rand.y][Rand.x] == '.')
			return Rand;  // Return space pos
	}
}

string Map::UserInput(string title) {
	sf::Text txtTitle(title, gBasicFont, 50);  // Title text
	txtTitle.setFillColor(C_WHITE);
	txtTitle.setPosition(gWidth / 2, 100);
	txtTitle.setStyle(sf::Text::Bold);
	txtTitle.setOrigin(txtTitle.getLocalBounds().width / 2, 0);  // Center x based on text width

	sf::Text txtPrompt("Map name:", gBasicFont);  // Prompt text
	txtPrompt.setFillColor(C_WHITE);
	txtPrompt.setPosition(gWidth / 2, 200);
	txtPrompt.setOrigin(txtPrompt.getLocalBounds().width / 2, 0);  // Center x based on text width

	sf::Text txtUser("", gBasicFont);  // User text
	txtUser.setFillColor(C_WHITE);
	txtUser.setPosition(gWidth / 2, 250);
	txtUser.setOrigin(txtUser.getLocalBounds().width / 2, 0);  // Center x based on text width

	string tempString{ "" };
	bool userInput{ true };
	while (userInput) {  // Main loop
		gWindow.clear();  // Clear screen between frames
		sf::Event event;  // Window events
		while (gWindow.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:  // If window closed
				gWindow.close();  // Exit window
				exit(0);  // Stop code
			case sf::Event::TextEntered:  // If user types something
				if (event.text.unicode < 128) {  // And its a valid key
					//cout << event.text.unicode << endl;  // Print unicode (for debugging)
					if (event.text.unicode == 13)  // If enter hit then stop input
						userInput = false;  // Break while loop
					else if (event.text.unicode == 8) {  // If backspace hit (and not empty)
						if (tempString.length() > 0)
							tempString.pop_back();  // Remove end character
					}
					else
						tempString += static_cast<char>(event.text.unicode);
					txtUser.setString(tempString);
					txtUser.setOrigin(txtUser.getLocalBounds().width / 2, 0);  // Center x based on text width
					cout << tempString << endl;
				}
			default:  // Pass
				break;
			}
		}

		gMap.Draw(100);  // Draw map (as background)
		gWindow.draw(txtTitle);  // Draw text
		gWindow.draw(txtPrompt);
		gWindow.draw(txtUser);
		gWindow.display();  // Show frame
	}
	gWindow.clear();
	return tempString;
}

Player::Player() {  // Player constructor
	map_block.push_back(gM_WALL);  // Blocked map movements
	MoveTo(RandSpace());  // Randomise start position
}

bool Player::IsAllowed() {  // Test if allowed to move to test_pos
	for (int i = 0; i < map_block.size(); i++) {
		if (gMap.currLevel[pos.y][pos.x] == map_block[i])
			return false;
	}
	return true;
}

void Player::Move(EInput key)  // Movement
{  // Movement
	if (key == EInput::eNone) // If none do nothing
		return;

	gMap.currLevel[pos.y][pos.x] = gM_SPACE;  // Remove player

	switch (key) {
	case EInput::eUp:  // Key pressed
		pos.y--;  // Move player
		if (!IsAllowed())  // If not allowed move back
			pos.y++;
		break;
	case EInput::eDown:
		pos.y++;
		if (!IsAllowed())
			pos.y--;
		break;
	case EInput::eLeft:
		pos.x--;
		if (!IsAllowed())
			pos.x++;
		break;
	case EInput::eRight:
		pos.x++;
		if (!IsAllowed())
			pos.x--;
		break;
	}

	if (gMap.currLevel[pos.y][pos.x] == gM_GOAL) {  // Check if hit goal
		cout << "You found goal number " << gNumGoals++ << " (Score: " << ceil(gClock.getElapsedTime().asSeconds()) * gNumGoals << ")" << endl;
		while (true) {  // Generate new goal
			sf::Vector2i Rand = RandSpace();
			gMap.currLevel[Rand.y][Rand.x] = 'G';
			break;
		}
	}
	else if (map_value == gM_PLAYER && gMap.currLevel[pos.y][pos.x] == gM_ENEMY || map_value == gM_ENEMY && gMap.currLevel[pos.y][pos.x] == gM_PLAYER) {  // Check if player hit enemy or enemy hit player
		cout << "An enemy got you! You lose. (Score: " << ceil(gClock.getElapsedTime().asSeconds()) * gNumGoals << ")" << endl;
		gMap.Draw(255);  // Update map to remove player
		gWindow.display();  // Draw to screen
		this_thread::sleep_for(2s);  // Sleep for n seconds
		gInMenu = true;  // Quit
	}

	gMap.currLevel[pos.y][pos.x] = map_value;  // Move
}

void Player::MoveTo(sf::Vector2i newPos) {  // Move self to new position
	pos = newPos;
	gMap.currLevel[pos.y][pos.x] = map_value;
}

Enemy::Enemy() {  // Convert player to enemy
	map_value = gM_ENEMY;
	gMap.currLevel[pos.y][pos.x] = map_value;
	map_block.push_back(gM_GOAL);
	map_block.push_back(gM_ENEMY);
}

void Map::UpdateEntities(Player& player, Enemy* enemies) {
	player.MoveTo(RandSpace());  // Move player, goal and enemies to new space
	while (true) {  // Generate new goal
		sf::Vector2i Rand = RandSpace();
		currLevel[Rand.y][Rand.x] = 'G';
		break;
	}
	for (int i = 0; i < gE_NUM; i++)
		enemies[i].MoveTo(RandSpace());
}

bool Map::Load() {  // Load map (player and enemies by ref)
	cout << endl << "Load map" << endl;  // Ask user for map name
	fileName = UserInput("Load map") + ".txt";  // Add file extension
	cout << "Loading map \"" << fileName << "\"" << endl;

	ifstream gameSave(fileName);  // Open file
	if (gameSave.fail()) {  // If error opening file
		cerr << "Failed to load \"" << fileName << "\"!" << endl;  // Return without saving
		return false;
	}

	int x{ 0 }, y{ 0 };
	while (!gameSave.eof()) {
		gameSave >> currLevel[y][x];  // Overwrite to new map
		x++;
		if (x >= gMAZE_X) {  // If end of x
			y++;  // Move to next line
			x = 0;
		}
		if (y > gMAZE_Y) {
			cerr << "Map is too tall!" << endl;
			break;
		}
	} // Continue to end of file

	gameSave.close();  // Close file

	cout << "Game loaded from \"" << fileName << "\"!" << endl;
	return true;
}

bool Map::Save() {  // Save map
	ofstream gameSave;
	cout << endl << "Save map" << endl;  // Ask user for map name
	fileName = UserInput("Save map") + ".txt";  // Add file extension

	gameSave.open(fileName);  // Open file
	if (gameSave.fail()) {  // If error opening file
		cerr << "Failed to open " << fileName << ", attemting to make new file..." << endl;
		ofstream gameSave(fileName);  // Create new map
		gameSave.open(fileName);  // Open new file
		if (gameSave.fail()) {  // If error opening new file
			cerr << "Failed to create \"" << fileName << "\", map could not be saved." << endl;
			return false;  // Return without saving
		}
	}

	for (int y = 0; y < gMAZE_Y; y++) {  // Loop through map
		for (int x = 0; x < gMAZE_X; x++) {
			if (currLevel[y][x] == gM_PLAYER || currLevel[y][x] == gM_ENEMY || currLevel[y][x] == gM_GOAL)
				gameSave << gM_SPACE << ' ';  // Remove player, enemy and goals as they are generated
			else
				gameSave << currLevel[y][x] << ' ';  // Write map character
		}
		gameSave << endl;  // Return after each map line
	}
	gameSave.close();  // Close file

	cout << "Game saved to \"" << fileName << "\"!" << endl;
	return true;
}

void Map::Draw(int alpha) {  // Draw map to screen
	sf::Color tempColour;
	for (int y = 0; y < gMAZE_Y; y++) {  // Loop through entire maze
		for (int x = 0; x < gMAZE_X; x++) {
			switch (currLevel[y][x]) {
			case gM_WALL:  // Wall
				tempColour = C_GREY;  // Grey
				break;
			case gM_GOAL:  // Goal
				tempColour = C_GREEN;  // Green
				break;
			case gM_PLAYER:  // Player
				tempColour = C_YELLOW;  // Yellow
				break;
			case gM_ENEMY:  // Enemy
				tempColour = C_RED;  // Red
				break;
			default:  // Other
				break;
			}

			tempColour.a = alpha;
			gRect.setFillColor(tempColour);  // Rect
			gCirc.setFillColor(tempColour);  // Cirle

			switch (currLevel[y][x]) {
			case gM_WALL:  // Wall
			case gM_GOAL:  // Goal
				gRect.setPosition(x * CELL_SIZE.x, y * CELL_SIZE.y);  // Square
				gWindow.draw(gRect);  // Draw to screen
				break;
			case gM_PLAYER:  // Player
			case gM_ENEMY:  // Enemy
				gCirc.setPosition(x * CELL_SIZE.x + (gRADIUS / 2), y * CELL_SIZE.y + (gRADIUS / 2));  // Circle
				gWindow.draw(gCirc);  // Draw to screen
				break;
			default:  // None
				break;
			}
		}
	}
}

int main()
{
	cout << "|--------- CONTROLS ---------" << endl
		<< "|-- Menu --" << endl
		<< "| [ Up ]      Scroll up" << endl
		<< "| [ Down ]    Scroll down" << endl
		<< "| [ Enter ]   Select option" << endl
		<< "| [ Escape ]  Unpause / resume" << endl
		<< "|" << endl
		<< "|-- Gameplay --" << endl
		<< "| [ Left ]    Move left" << endl
		<< "| [ Right ]   Move right" << endl
		<< "| [ Up ]      Move up" << endl
		<< "| [ Down ]    Move down" << endl
		<< "| [ Escape ]  Pause game" << endl
		<< "|-----------------------------" << endl << endl;

	const int FPS{ 80 }, E_SPEED{ 5 };  // FPS, Amount of enemies, Enemy random chance
	bool menuFlash{ true };
	EMenu menuOpt{ EMenu::eStart };

	if (!gBasicFont.loadFromFile("basic.ttf"))  // Load custom font
		exit(0);

	sf::Text txtScore("Score:", gBasicFont);  // Score text
	txtScore.setFillColor(C_WHITE);
	txtScore.setPosition(0, 0);

	sf::Text txtTitle("Main Menu", gBasicFont, 50);  // Title text
	txtTitle.setFillColor(C_WHITE);
	txtTitle.setPosition(gWidth / 2, 100);
	txtTitle.setStyle(sf::Text::Bold);
	txtTitle.setOrigin(txtTitle.getLocalBounds().width / 2, 0);  // Center x based on text width

	sf::Text txtStart("Start Game", gBasicFont);  // Start Game text
	txtStart.setFillColor(C_WHITE);
	txtStart.setPosition(gWidth / 2, 200);
	txtStart.setOrigin(txtStart.getLocalBounds().width / 2, 0);  // Center x based on text width

	sf::Text txtLoad("Load Game", gBasicFont);  // Load Game text
	txtLoad.setFillColor(C_GREY);
	txtLoad.setPosition(gWidth / 2, 250);
	txtLoad.setOrigin(txtLoad.getLocalBounds().width / 2, 0);  // Center x based on text width

	sf::Text txtSave("Save Game", gBasicFont);  // Save Game text
	txtSave.setFillColor(C_GREY);
	txtSave.setPosition(gWidth / 2, 300);
	txtSave.setOrigin(txtSave.getLocalBounds().width / 2, 0);  // Center x based on text width

	sf::Text txtQuit("Quit", gBasicFont);  // Quit Game text
	txtQuit.setFillColor(C_GREY);
	txtQuit.setPosition(gWidth / 2, 350);
	txtQuit.setOrigin(txtQuit.getLocalBounds().width / 2, 0);  // Center x based on text width

	sf::Text txtMap("Current map: None", gBasicFont);  // Current Map text
	txtMap.setFillColor(C_WHITE);
	txtMap.setPosition(gWidth / 2, 500);
	txtMap.setOrigin(txtMap.getLocalBounds().width / 2, 0);  // Center x based on text width

	srand(static_cast<unsigned int>(time(NULL)));  // Set random seed
	gRect.setSize(CELL_SIZE);  // Set rect width
	gCirc.setRadius(gRADIUS);  // Set radius
	
	//gMap.Load();

	Player player;  // Player
	Enemy enemies[gE_NUM];  // Array of enemies
	gGoal = RandSpace();  // Random goal start
	gMap.currLevel[gGoal.y][gGoal.x] = gM_GOAL;

	gClock.restart();  // Start score timer
	while (gWindow.isOpen()) {  // Main loop
		this_thread::sleep_for(chrono::milliseconds(FPS));  // Window FPS
		gWindow.clear();  // Clear window between frames

		sf::Event event;  // Window events
		while (gWindow.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:  // If window closed
				gWindow.close();  // Exit window
				exit(0);  // Stop code
			case sf::Event::TextEntered:  // If user types something
				if (event.text.unicode < 128)  // And its a valid key
					cout << static_cast<char>(event.text.unicode) << endl;
			default:  // Pass
				break;
			}
		}

		if (gInMenu) {  // If in menus
			if (static_cast<int>(gClock.getElapsedTime().asSeconds()) % 2 == 0 && !menuFlash ||
				static_cast<int>(gClock.getElapsedTime().asSeconds()) % 2 == 1 && menuFlash) {
				switch (menuOpt) {
				case EMenu::eStart:
					txtStart.setFillColor(menuFlash ? C_WHITE : C_LGREEN);
					break;
				case EMenu::eLoad:
					txtLoad.setFillColor(menuFlash ? C_WHITE : C_LGREEN);
					break;
				case EMenu::eSave:
					txtSave.setFillColor(menuFlash ? C_WHITE : C_LGREEN);
					break;
				case EMenu::eQuit:
					txtQuit.setFillColor(menuFlash ? C_WHITE : C_LGREEN);
					break;
				}
				menuFlash = !menuFlash;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {  // Keyboard up
				switch (menuOpt) {
				case EMenu::eLoad:
					menuOpt = EMenu::eStart;
					txtStart.setFillColor(C_WHITE);
					txtLoad.setFillColor(C_GREY);
					break;
				case EMenu::eSave:
					menuOpt = EMenu::eLoad;
					txtLoad.setFillColor(C_WHITE);
					txtSave.setFillColor(C_GREY);
					break;
				case EMenu::eQuit:
					menuOpt = EMenu::eSave;
					txtSave.setFillColor(C_WHITE);
					txtQuit.setFillColor(C_GREY);
					break;
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { // Keyboard down
				switch (menuOpt) {
				case EMenu::eStart:
					menuOpt = EMenu::eLoad;
					txtLoad.setFillColor(C_WHITE);
					txtStart.setFillColor(C_GREY);
					break;
				case EMenu::eLoad:
					menuOpt = EMenu::eSave;
					txtSave.setFillColor(C_WHITE);
					txtLoad.setFillColor(C_GREY);
					break;
				case EMenu::eSave:
					menuOpt = EMenu::eQuit;
					txtQuit.setFillColor(C_WHITE);
					txtSave.setFillColor(C_GREY);
					break;
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) { // Keyboard enter
				switch (menuOpt) {
				case EMenu::eStart:
					gInMenu = false;
					break;
				case EMenu::eLoad:
					if (gMap.Load())
						gMap.UpdateEntities(player, enemies);
					break;
				case EMenu::eSave:
					gMap.Save();
					break;
				case EMenu::eQuit:
					exit(0);
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {  // Keyboard escape
				gClock.restart();  // Restart timer (to not get points while paused)
				gInMenu = false;  // Unpause game
			}

			// Menu loop
			gMap.Draw(100);  // Draw map (as background)
			gWindow.draw(txtTitle);  // Draw text
			gWindow.draw(txtStart);
			gWindow.draw(txtLoad);
			gWindow.draw(txtSave);
			gWindow.draw(txtQuit);
			gWindow.draw(txtMap);
		}
		else {  // If not in menus (playing game)
			EInput key{ EInput::eNone };  // Keyboard input
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  // Keyboard left
				key = EInput::eLeft;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))  // Keyboard right
				key = EInput::eRight;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))  // Keyboard up
				key = EInput::eUp;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) // Keyboard down
				key = EInput::eDown;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))  // Keyboard escape
				gInMenu = true;  // Pause game

			// Gameplay loop
			for (int i = 0; i < gE_NUM; i++) {  // Update enemies
				if (rand() % E_SPEED == 0)  // Decide to move
					enemies[i].Move(static_cast<EInput>(rand() % 4 + 1));  // Random direction (as EInput)
			}
			player.Move(key);  // Player movement

			txtScore.setString("Score: " + to_string(static_cast<int>(ceil(gClock.getElapsedTime().asSeconds()) * gNumGoals)));

			gMap.Draw(255);  // Draw map
			gWindow.draw(txtScore);  // Draw text
		}

		gWindow.display();  // Show complete frame to user
	}
	return 0;
}
