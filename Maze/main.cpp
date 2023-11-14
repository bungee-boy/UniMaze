/*
	GPF Week 6 - Maze Start Code
*/

#include <iostream>
#include <vector>
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
int radius = (WIDTH > HEIGHT ? WIDTH : HEIGHT) / 3;  // Circle radius
int numGoals{ 1 };  // Number of goals (counter)
Pos Goal;  // Goal position

char map[kMazeX][kMazeY] = {
	{ 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W' },
	{ 'W', '.', '.', '.', '.', '.', '.', '.', '.', 'W', 'W', '.', '.', '.', '.', '.', '.', '.', '.', 'W' },
	{ 'W', '.', 'W', 'W', 'W', 'W', 'W', 'W', '.', 'W', '.', '.', 'W', 'W', 'W', 'W', 'W', 'W', '.', 'W' },
	{ 'W', '.', 'W', '.', '.', '.', 'W', '.', '.', 'W', '.', 'W', 'W', '.', '.', '.', '.', '.', '.', 'W' },
	{ 'W', '.', 'W', '.', 'W', '.', 'W', '.', 'W', 'W', '.', 'W', 'W', '.', 'W', 'W', 'W', 'W', '.', 'W' },
	{ 'W', '.', 'W', '.', 'W', '.', '.', '.', '.', '.', '.', '.', 'W', '.', 'W', '.', '.', '.', '.', 'W' },
	{ 'W', '.', 'W', '.', 'W', 'W', '.', 'W', 'W', 'W', 'W', '.', 'W', '.', '.', '.', 'W', 'W', 'W', 'W' },
	{ 'W', '.', 'W', '.', 'W', 'W', '.', 'W', 'W', 'W', 'W', '.', 'W', '.', 'W', '.', 'W', 'W', 'W', 'W' },
	{ 'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W', '.', '.', '.', 'W', '.', '.', '.', 'W', 'W' },
	{ 'W', '.', 'W', 'W', '.', 'W', 'W', 'W', '.', 'W', 'W', 'W', '.', 'W', 'W', 'W', 'W', '.', 'W', 'W' },
	{ 'W', '.', 'W', 'W', '.', 'W', 'W', 'W', '.', 'W', 'W', 'W', '.', 'W', 'W', 'W', 'W', '.', 'W', 'W' },
	{ 'W', '.', 'W', '.', '.', 'W', 'W', 'W', '.', 'W', 'W', '.', '.', '.', '.', '.', '.', '.', '.', 'W' },
	{ 'W', '.', 'W', '.', 'W', 'W', 'W', 'W', '.', '.', '.', '.', 'W', '.', 'W', 'W', 'W', 'W', '.', 'W' },
	{ 'W', '.', 'W', '.', '.', '.', 'W', '.', '.', 'W', 'W', '.', 'W', '.', 'W', '.', '.', '.', '.', 'W' },
	{ 'W', '.', 'W', '.', 'W', '.', 'W', '.', 'W', 'W', 'W', '.', 'W', '.', 'W', '.', 'W', 'W', 'W', 'W' },
	{ 'W', '.', 'W', '.', 'W', '.', '.', '.', '.', 'W', '.', '.', 'W', '.', 'W', '.', '.', '.', 'W', 'W' },
	{ 'W', '.', 'W', '.', 'W', 'W', 'W', 'W', '.', 'W', '.', 'W', 'W', '.', '.', '.', 'W', '.', '.', 'W' },
	{ 'W', '.', 'W', '.', 'W', 'W', 'W', 'W', '.', 'W', '.', 'W', 'W', 'W', 'W', '.', 'W', 'W', '.', 'W' },
	{ 'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W' },
	{ 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W' },
};

Pos RandSpace() {  // Find random space
	Pos Rand;
	while (true) {  // Search until space found
		Rand.x = rand() % (kMazeX - 2) + 1;  // Efficient rand (doesnt check outer walls)
		Rand.y = rand() % (kMazeY - 2) + 1;
		if (map[Rand.y][Rand.x] == '.') {
			return Rand;  // Return space pos
		}
	}
}

void NewGoal() {  // Randomise new goal
	while (true) {
		Pos Rand = RandSpace();
		map[Rand.y][Rand.x] = 'G';
		break;
	}
}

Player::Player() {  // Player constructor
	pos = RandSpace();  // Randomise start position
	map[pos.y][pos.x] = 'P';
}

void Player::Move(EKeyPressed key)  // Player move
{  // Movement
	if (key == EKeyPressed::eNone || pos.x > kMazeX && pos.y > kMazeY) { // If none or dead do nothing
		return;
	}
	map[pos.y][pos.x] = '.';  // Remove player

	switch (key) {
	case EKeyPressed::eUp:  // Key pressed
		pos.y--;  // Move player
		if (map[pos.y][pos.x] == 'W')  // If wall move back
			pos.y++;
		break;
	case EKeyPressed::eDown:
		pos.y++;
		if (map[pos.y][pos.x] == 'W')
			pos.y--;
		break;
	case EKeyPressed::eLeft:
		pos.x--;
		if (map[pos.y][pos.x] == 'W')
			pos.x++;
		break;
	case EKeyPressed::eRight:
		pos.x++;
		if (map[pos.y][pos.x] == 'W')
			pos.x--;
		break;
	}

	if (map[pos.y][pos.x] == 'G') {  // Check if player hit goal
		cout << "You found goal number " << numGoals++ << " (Score: " << GetElapsedTime() << ")" << endl;
		NewGoal();  // Generate new goal
		StartClock(); // Restart timer for next score
	}
	else if (map[pos.y][pos.x] == 'E') {  // Check if player hit enemy
		cout << "An enemy got you! You lose. (Score: " << GetElapsedTime() << ")" << endl;
		pos.x = kMazeX + 1;
		pos.y = kMazeY + 1;
	}
	map[pos.y][pos.x] = 'P';  // Move player
}

void Enemy::Move(Player& player) {  // Enemy move
	if (rand() % E_SPEED == 0) {  // Decide to move
		int direction = rand() % 4;  // Decide direction
		map[pos.y][pos.x] = '.';  // Remove from map
		switch (direction) {
		case 0:  // Up
			pos.y--;  // Move
			if (map[pos.y][pos.x] == 'W' || map[pos.y][pos.x] == 'G')  // If wall or goal
				pos.y++;  // Move back
			break;
		case 1:  // Down
			pos.y++;
			if (map[pos.y][pos.x] == 'W' || map[pos.y][pos.x] == 'G')
				pos.y--;
			break;
		case 2:  // Left
			pos.x--;
			if (map[pos.y][pos.x] == 'W' || map[pos.y][pos.x] == 'G')
				pos.x++;
			break;
		case 3:  // Right
			pos.x++;
			if (map[pos.y][pos.x] == 'W' || map[pos.y][pos.x] == 'G')
				pos.x--;
			break;
		}

		if (map[pos.y][pos.x] == 'P') {  // Check if enemy hit player
			cout << "An enemy got you! You lose. (Score: " << GetElapsedTime() << ")" << endl;
			player.pos.x = kMazeX + 1;
			player.pos.y = kMazeY + 1;
		}

		map[pos.y][pos.x] = 'E';  // Set new position on map
	}
}

void DrawMaze() {  // Draw the maze
	for (int y = 0; y < kMazeY; y++) {  // Loop through entire maze
		for (int x = 0; x < kMazeX; x++) {
			switch (map[y][x]) {
			case 'W':  // Wall
				ChangeColour(100, 100, 100);  // Grey
				break;
			case 'G':  // Goal
				ChangeColour(50, 255, 50);  // Green
				break;
			case 'P':  // Player
				ChangeColour(255, 255, 50);  // Yellow
				break;
			case 'E':  // Enemy
				ChangeColour(255, 50, 50);  // Red
				break;
			default:  // Other
				break;
			}

			switch (map[y][x]) {
			case 'W':  // Wall
			case 'G':  // Goal
				DrawRectangle(x * WIDTH, y * HEIGHT, WIDTH, HEIGHT);  // Square
				break;
			case 'P':  // Player
			case 'E':  // Enemy
				DrawCircle(x * WIDTH + (radius / 2), y * HEIGHT + (radius / 2), radius);  // Circle
				break;
			default:  // None
				break;
			}
		}
	}
}

int main()
{
	srand(time(NULL));  // Set random seed
	Player player;  // Player
	Enemy enemies[E_NUM];  // Array of enemies
	Goal = RandSpace();  // Random goal start
	map[Goal.y][Goal.x] = 'G';

	StartClock();  // Start score timer
	while (UpdateFramework())
	{
		player.Move(GetLastKeyPressed());  // Player movement
		for (int i = 0; i < E_NUM; i++) {  // Update enemies
			enemies[i].Move(player);
		}
		DrawMaze();
	}

	return 0;
}
