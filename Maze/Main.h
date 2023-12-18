#pragma once

const int gWidth{ 800 }, gHeight{ 800 };  // Screen x, Screen y
const int gMAZE_X{ 20 }, gMAZE_Y{ 20 };  // Maze size as constants
const int gE_NUM{ 5 };  // Number of enemies

const sf::Vector2f CELL_SIZE{gWidth / gMAZE_X, gHeight / gMAZE_Y};  // Cell size
const sf::Color C_RED{ 255, 50, 50, 255}, C_GREEN{ 50, 255, 50, 255 }, C_YELLOW{ 255, 255, 50, 255 };  // Colours
const sf::Color C_WHITE{ 255, 255, 255, 255 }, C_LGREEN{ 105, 255, 105, 255 }, C_GREY{ 100, 100, 100, 255 }, C_BLACK{ 0, 0, 0, 255 };

const float gRADIUS{ (CELL_SIZE.x > CELL_SIZE.y ? CELL_SIZE.x : CELL_SIZE.y) / 3 };  // Circle radius

const char gM_PLAYER{ 'P' }, gM_ENEMY{ 'E' }, gM_WALL{ 'W' }, gM_GOAL{ 'G' }, gM_SPACE{ '.' };  // Map characters

sf::CircleShape gCirc;  // Load circle class
sf::RectangleShape gRect;  // Load rect classextern const int gWidth, gHeight;
sf::Clock gClock;  // Clock for score timer
sf::RenderWindow gWindow(sf::VideoMode(gWidth, gHeight), "Maze");  // Initialise window
sf::Vector2i gGoal;  // Goal position
sf::Font gBasicFont;  // Font class

int gNumGoals{ 1 }, gHighScore{ 0 };  // Number of goals, highscore
bool gInMenu{ true };  // User is in menu or gameplay

enum class EInput {
	eNone,
	eLeft,  // Left arrow
	eRight,  // Right arrow
	eUp,  // Up arrow
	eDown,  // Down arrow
	eEnter,  // Return key
	eEscape  // Escape key
};

enum class EMenu {
	eStart,
	eLoad,
	eSave,
	eQuit
};

sf::Vector2i RandSpace();

class Player {  // Entity base class
public:
	Player();
	void Move(EInput key);
	void MoveTo(sf::Vector2i pos);
protected:
	sf::Vector2i pos;  // Draw position
	std::vector<char> map_block;  // Disallowed moves
	char map_value{ gM_PLAYER };  // Map identifier
	bool IsAllowed();
};

class Enemy: public Player {  // Inherit Entity using polymorphism
public:
	Enemy();
};

class Map {
public:
	std::string fileName{ "" };
	char currLevel[gMAZE_X][gMAZE_Y]{
		{'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},
		{'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W'},
		{'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W'},
		{'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W'},
		{'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W'},
		{'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W'},
		{'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W'},
		{'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W'},
		{'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W'},
		{'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W'},
		{'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W'},
		{'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W'},
		{'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W'},
		{'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W'},
		{'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W'},
		{'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W'},
		{'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W'},
		{'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W'},
		{'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W'},
		{'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'} };
	std::string UserInput(std::string title);
	void UpdateEntities(Player& player, Enemy* enemies);
	bool Load();
	bool Save();
	void Draw(int alpha);
};

Map gMap;
