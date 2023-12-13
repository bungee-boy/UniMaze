#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <time.h>
#include <stdio.h>


#include "Framework.h"
#include "Main.h"

using namespace std;

int gScreenWidth{ 800 }, gScreenHeight{ 800 };  // Screen dimensions

int gTimeDelayMS{ 100 };  // Delay to slow things down

const int kMazeX{ 20 }, kMazeY{ 20 };  // Maze size as constants
const int E_NUM{ 5 }, E_SPEED{ 5 };  // Number of enemies and movement chance
const int WIDTH{ gScreenWidth / kMazeX }, HEIGHT{ gScreenHeight / kMazeY };  // Cell size
const int gRadius = (WIDTH > HEIGHT ? WIDTH : HEIGHT) / 3;  // Circle radius
int numGoals{ 1 };  // Number of goals (counter)
Pos gGoal;  // Goal position
Map gMap;  // Map

void Map::Load(Player& player, Enemy* enemies) {  // Load map (player and enemies by ref)
	cout << endl << "Load map" << endl << "Enter map name: ";  // Ask user for map name
	cin >> fileName;
	fileName += ".txt";  // Add file extension

	ifstream gameSave(fileName);  // Open file
	if (gameSave.fail()) {  // If error opening file
		cerr << "Failed to open " << fileName << '!' << endl;  // Return without saving
		return;
	}

	int x{ 0 }, y{ 0 };
	while (!gameSave.eof()) {
		gameSave >> currLevel[y][x];  // Overwrite to new map
		x++;
		if (x >= M_SIZE_X) {  // If end of x
			y++;  // Move to next line
			x = 0;
		}
		if (y > M_SIZE_Y) {
			cerr << "Map is too tall!" << endl;
			break;
		}
	} // Continue to end of file

	gameSave.close();  // Close file

	player.MoveTo(RandSpace());  // Move player, goal and enemies to new space
	NewGoal();
	for (int i = 0; i < E_NUM; i++)
		enemies[i].MoveTo(RandSpace());

	cout << "Game loaded from \"" << fileName << "\"!" << endl;
}

void Map::Save() {  // Save map
	ofstream gameSave;
	cout << endl << "Save map" << endl << "Enter map name: ";  // Ask user for map name
	cin >> fileName;
	fileName += ".txt";  // Add file extension

	gameSave.open(fileName);  // Open file
	if (gameSave.fail()) {  // If error opening file
		cerr << "Failed to open " << fileName << ", attemting to make new file..." << endl;
		ofstream gameSave(fileName);  // Create new map
		gameSave.open(fileName);  // Open new file
		if (gameSave.fail()) {  // If error opening new file
			cerr << "Failed to create \"" << fileName << "\", map could not be saved." << endl;
			return;  // Return without saving
		}
	}

	for (int y = 0; y < M_SIZE_Y; y++) {  // Loop through map
		for (int x = 0; x < M_SIZE_X; x++) {
			if (currLevel[y][x] == M_PLAYER || currLevel[y][x] == M_ENEMY || currLevel[y][x] == M_GOAL)
				gameSave << M_SPACE << ' ';  // Remove player, enemy and goals as they are generated
			else
				gameSave << currLevel[y][x] << ' ';  // Write map character
		}
		gameSave << endl;  // Return after each map line
	}
	gameSave.close();  // Close file
	cout << "Game saved to \"" << fileName << "\"!" << endl;
}

void Map::Draw() {  // Draw map to screen
	for (int y = 0; y < kMazeY; y++) {  // Loop through entire maze
		for (int x = 0; x < kMazeX; x++) {
			switch (currLevel[y][x]) {
			case M_WALL:  // Wall
				ChangeColour(100, 100, 100);  // Grey
				break;
			case M_GOAL:  // Goal
				ChangeColour(50, 255, 50);  // Green
				break;
			case M_PLAYER:  // Player
				ChangeColour(255, 255, 50);  // Yellow
				break;
			case M_ENEMY:  // Enemy
				ChangeColour(255, 50, 50);  // Red
				break;
			default:  // Other
				break;
			}

			switch (currLevel[y][x]) {
			case M_WALL:  // Wall
			case M_GOAL:  // Goal
				DrawRectangle(x * WIDTH, y * HEIGHT, WIDTH, HEIGHT);  // Square
				break;
			case M_PLAYER:  // Player
			case M_ENEMY:  // Enemy
				DrawCircle(x * WIDTH + (gRadius / 2), y * HEIGHT + (gRadius / 2), gRadius);  // Circle
				break;
			default:  // None
				break;
			}
		}
	}
}

Pos RandSpace() {  // Find random space
	Pos Rand;
	while (true) {  // Search until space found
		Rand.x = rand() % (kMazeX - 2) + 1;  // Efficient rand (doesnt check outer walls)
		Rand.y = rand() % (kMazeY - 2) + 1;
		if (gMap.currLevel[Rand.y][Rand.x] == '.') {
			return Rand;  // Return space pos
		}
	}
}

void NewGoal() {  // Randomise new goal
	while (true) {
		Pos Rand = RandSpace();
		gMap.currLevel[Rand.y][Rand.x] = 'G';
		break;
	}
}

Player::Player() {  // Player constructor
	pos = RandSpace();  // Randomise start position
	map_block.push_back(M_WALL);  // Blocked map movements
	gMap.currLevel[pos.y][pos.x] = map_value;
}

Enemy::Enemy() {
	map_block.push_back(M_GOAL);  // Add goal to vector of blocked movements
	map_value = M_ENEMY;
	gMap.currLevel[pos.y][pos.x] = map_value;
}

bool Player::IsAllowed() {  // Test if allowed to move to test_pos
	for (int i = 0; i < map_block.size(); i++) {
		if (gMap.currLevel[pos.y][pos.x] == map_block[i]) {
			return false;
		}
	}
	return true;
}

void Player::Move(EKeyPressed key)  // Movement
{  // Movement
	if (key == EKeyPressed::eNone) // If none do nothing
		return;

	gMap.currLevel[pos.y][pos.x] = M_SPACE;  // Remove player

	switch (key) {
	case EKeyPressed::eUp:  // Key pressed
		pos.y--;  // Move player
		if (!IsAllowed())  // If wall move back
			pos.y++;
		break;
	case EKeyPressed::eDown:
		pos.y++;
		if (!IsAllowed())
			pos.y--;
		break;
	case EKeyPressed::eLeft:
		pos.x--;
		if (!IsAllowed())
			pos.x++;
		break;
	case EKeyPressed::eRight:
		pos.x++;
		if (!IsAllowed())
			pos.x--;
		break;
	}

	if (gMap.currLevel[pos.y][pos.x] == M_GOAL) {  // Check if hit goal
		cout << "You found goal number " << numGoals++ << " (Score: " << ceil(GetElapsedTime()) * numGoals << ")" << endl;
		NewGoal();  // Generate new goal
	}
	else if (gMap.currLevel[pos.y][pos.x] == M_ENEMY) {  // Check if hit enemy
		cout << "An enemy got you! You lose. (Score: " << ceil(GetElapsedTime()) * numGoals << ")" << endl;
		gMap.Draw();  // Update map to remove player
		UpdateFramework();  // Draw to screen
		this_thread::sleep_for(3s);  // Sleep for n seconds
		exit(0);  // Quit
	}
	gMap.currLevel[pos.y][pos.x] = map_value;  // Move
}

void Player::MoveTo(Pos newPos) {  // Move self to new position
	pos = newPos;
	gMap.currLevel[pos.y][pos.x] = map_value;
}

int main()
{
	srand(static_cast<unsigned int>(time(NULL)));  // Set random seed
	Player player;  // Player
	Enemy enemies[E_NUM];  // Array of enemies
	gGoal = RandSpace();  // Random goal start
	gMap.currLevel[gGoal.y][gGoal.x] = M_GOAL;

	StartClock();  // Start score timer
	while (UpdateFramework())
	{
		EKeyPressed key = GetLastKeyPressed();  // Keyboard input

		switch (key) {  // Map save & load
		case EKeyPressed::eSave:
			gMap.Save();
			break;
		case EKeyPressed::eLoad:
			gMap.Load(player, enemies);
			break;
		}

		player.Move(key);  // Player movement

		for (int i = 0; i < E_NUM; i++) {  // Update enemies
			if (rand() % E_SPEED == 0)  // Decide to move
				enemies[i].Move(static_cast<EKeyPressed>(rand() % 4 + 1));  // Random direction (as EKeyPressed)
		}
		gMap.Draw();
	}

	return 0;
}
