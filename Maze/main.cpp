#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <stdio.h>

#include "Framework.h"
#include "Main.h"

using namespace std;

int gScreenWidth{ 800 }, gScreenHeight{ 800 };  // Screen dimensions

int gTimeDelayMS{ 100 };  // Delay to slow things down

const int kMazeX{ 20 }, kMazeY{ 20 };  // Maze size as constants
const int E_NUM{ 1 }, E_SPEED{ 5 };  // Number of enemies and movement chance
const int WIDTH{ gScreenWidth / kMazeX }, HEIGHT{ gScreenHeight / kMazeY };  // Cell size
int radius = (WIDTH > HEIGHT ? WIDTH : HEIGHT) / 3;  // Circle radius
int numGoals{ 1 };  // Number of goals (counter)
Pos Goal;  // Goal position

Map::Map() {  // Map constructor
}

void Map::Load() {  // Load map
	ifstream gameSave(fileName);  // Open file

	if (gameSave.fail()) {  // If error opening file
		cerr << "Failed to open " << fileName << '!' << endl;  // Return without saving
		return;
	}

	char currLevel[M_SIZE_X][M_SIZE_Y];  // Clear currLevel
	int x{ 0 }, y{ 0 };
	while (!gameSave.eof()) {
		gameSave >> currLevel[y][x];  // Extract into currLevel
		x++;
		if (x >= M_SIZE_X) {  // If end of x
			y++;  // Move to next line
			x = 0;
		}
		if (y >= M_SIZE_Y) {
			cerr << "Map is too tall!" << endl;
			break;
		}
	} // Continue to end of file

	/*for (int y = 0; y < M_SIZE_Y; y++) {  // Move players and enemies to new positions
		for (int x = 0; x < M_SIZE_X; x++) {
			switch (currLevel[y][x]) {
			case M_PLAYER:

			}
		}
	}*/

	gameSave.close();  // Close file
	cout << "Game loaded!" << endl;
}

void Map::Save() {  // Save map
	ofstream gameSave;
	gameSave.open(fileName);  // Open file

	if (gameSave.fail()) {  // If error opening file
		cerr << "Failed to open " << fileName << '!' << endl;  // Return without saving
		return;
	}

	for (int y = 0; y < M_SIZE_Y; y++) {  // Loop through map
		for (int x = 0; x < M_SIZE_X; x++) {
			gameSave << currLevel[y][x] << " ";  // Write map character
		}
		gameSave << endl;  // Return after each map line
	}
	gameSave.close();  // Close file
	cout << "Game saved!" << endl;
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
				DrawCircle(x * WIDTH + (radius / 2), y * HEIGHT + (radius / 2), radius);  // Circle
				break;
			default:  // None
				break;
			}
		}
	}
}

void Map::Update(EKeyPressed key) {  // Input to save and load
	switch (key) {
	case EKeyPressed::eSave:
		Save();
		break;
	case EKeyPressed::eLoad:
		Load();
		break;
	}
}

Map map;


Pos RandSpace() {  // Find random space
	Pos Rand;
	while (true) {  // Search until space found
		Rand.x = rand() % (kMazeX - 2) + 1;  // Efficient rand (doesnt check outer walls)
		Rand.y = rand() % (kMazeY - 2) + 1;
		if (map.currLevel[Rand.y][Rand.x] == '.') {
			return Rand;  // Return space pos
		}
	}
}

void NewGoal() {  // Randomise new goal
	while (true) {
		Pos Rand = RandSpace();
		map.currLevel[Rand.y][Rand.x] = 'G';
		break;
	}
}

Player::Player() {  // Player constructor
	pos = RandSpace();  // Randomise start position
	map.currLevel[pos.y][pos.x] = map_value;
}

Enemy::Enemy() {
	map_block[2] = M_WALL, M_GOAL;
	map_value = M_ENEMY;
	map.currLevel[pos.y][pos.x] = map_value;
}

bool Player::IsAllowed() {  // Test if allowed to move to test_pos
	for (int i = 0; i < sizeof(map_block); i++) {
		if (map.currLevel[pos.y][pos.x] == map_block[i]) {
			return false;
		}
	}
	return true;
}

void Player::Move(EKeyPressed key)  // Movement
{  // Movement
	if (key == EKeyPressed::eNone) // If none do nothing
		return;

	map.currLevel[pos.y][pos.x] = M_SPACE;  // Remove player

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

	if (map.currLevel[pos.y][pos.x] == M_GOAL) {  // Check if hit goal
		cout << "You found goal number " << numGoals++ << " (Score: " << GetElapsedTime() << ")" << endl;
		NewGoal();  // Generate new goal
		StartClock(); // Restart timer for next score
	}
	else if (map.currLevel[pos.y][pos.x] == M_ENEMY) {  // Check if hit enemy
		cout << "An enemy got you! You lose. (Score: " << GetElapsedTime() << ")" << endl;  // Quit game
		exit(0);
	}
	map.currLevel[pos.y][pos.x] = map_value;  // Move
}


int main()
{
	srand(time(0));  // Set random seed
	//Map map;  // Map
	Player player;  // Player
	Enemy enemies[E_NUM];  // Array of enemies
	Goal = RandSpace();  // Random goal start
	map.currLevel[Goal.y][Goal.x] = M_GOAL;

	StartClock();  // Start score timer
	while (UpdateFramework())
	{
		EKeyPressed key = GetLastKeyPressed();  // Keyboard input
		map.Update(key);  // Map save & load
		player.Move(key);  // Player movement

		for (int i = 0; i < E_NUM; i++) {  // Update enemies
			if (rand() % E_SPEED == 0)  // Decide to move
				enemies[i].Move(static_cast<EKeyPressed>(rand() % 4 + 1));  // Random direction (as EKeyPressed)
		}
		map.Draw();
	}

	return 0;
}
