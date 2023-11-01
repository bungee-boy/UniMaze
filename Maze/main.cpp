/*
	GPF Week 6 - Maze Start Code
*/

#include <iostream>
#include <vector>
#include <time.h>
#include <stdio.h>

#include "Framework.h"
using namespace std;

int gScreenWidth{ 800 }, gScreenHeight{ 800 };  // Screen dimensions

int gTimeDelayMS{ 100 };  // Delay to slow things down

constexpr int kMazeX{ 20 }, kMazeY{ 20 };  // Maze size as constants

const int numEnemies{ 5 }, enemySpeed{ 25 };  // Number of enemies and movement chance
const int cellWidth{ gScreenWidth / kMazeX }, cellHeight{ gScreenHeight / kMazeY };  // Cell size
int radius = (cellWidth > cellHeight ? cellWidth : cellHeight) / 3;  // Circle radius
int numGoals{ 1 };  // Number of goals (counter)

char map[kMazeX][kMazeY] = {
	{ 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W' },
	{ 'W', '.', '.', '.', '.', 'W', '.', '.', '.', 'W', 'W', '.', '.', '.', '.', 'W', '.', '.', '.', 'W' },
	{ 'W', '.', 'W', 'W', 'W', 'W', 'W', 'W', '.', 'W', '.', '.', 'W', 'W', 'W', 'W', 'W', 'W', '.', 'W' },
	{ 'W', '.', 'W', '.', '.', '.', 'W', '.', '.', 'W', '.', 'W', 'W', '.', 'W', '.', '.', '.', '.', 'W' },
	{ 'W', '.', 'W', '.', 'W', '.', 'W', '.', 'W', 'W', '.', 'W', 'W', '.', 'W', 'W', 'W', '.', 'W', 'W' },
	{ 'W', '.', 'W', '.', 'W', '.', '.', '.', '.', '.', '.', '.', 'W', '.', 'W', '.', '.', '.', '.', 'W' },
	{ 'W', '.', 'W', '.', 'W', 'W', 'W', '.', '.', 'W', 'W', '.', 'W', '.', '.', '.', 'W', '.', '.', 'W' },
	{ 'W', '.', 'W', '.', 'W', '.', 'W', 'W', 'W', 'W', 'W', '.', 'W', '.', 'W', '.', 'W', 'W', 'W', 'W' },
	{ 'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W', '.', '.', '.', 'W', '.', '.', '.', '.', 'W' },
	{ 'W', '.', 'W', '.', 'W', '.', 'W', 'W', '.', 'W', 'W', 'W', '.', 'W', 'W', 'W', 'W', '.', 'W', 'W' },
	{ 'W', '.', 'W', 'W', 'W', 'W', 'W', 'W', '.', 'W', 'W', 'W', '.', 'W', '.', 'W', 'W', '.', 'W', 'W' },
	{ 'W', '.', '.', '.', '.', 'W', '.', '.', '.', 'W', 'W', '.', '.', '.', '.', 'W', '.', '.', '.', 'W' },
	{ 'W', '.', 'W', 'W', 'W', 'W', 'W', 'W', '.', '.', '.', '.', 'W', '.', 'W', 'W', 'W', 'W', '.', 'W' },
	{ 'W', '.', 'W', '.', '.', '.', 'W', '.', '.', 'W', 'W', '.', 'W', '.', 'W', '.', '.', '.', '.', 'W' },
	{ 'W', '.', 'W', '.', 'W', '.', 'W', '.', 'W', 'W', 'W', '.', 'W', '.', 'W', '.', 'W', 'W', 'W', 'W' },
	{ 'W', '.', 'W', '.', 'W', '.', '.', '.', '.', '.', 'W', '.', 'W', '.', 'W', '.', '.', '.', 'W', 'W' },
	{ 'W', '.', 'W', '.', 'W', 'W', 'W', '.', '.', 'W', 'W', '.', 'W', '.', '.', '.', 'W', '.', '.', 'W' },
	{ 'W', '.', 'W', '.', 'W', '.', 'W', 'W', 'W', 'W', '.', '.', 'W', 'W', 'W', '.', 'W', 'W', '.', 'W' },
	{ 'W', '.', '.', '.', '.', '.', '.', '.', '.', 'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W' },
	{ 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W' },
};

struct pos {
	int x{ 0 };
	int y{ 0 };
};

pos Player, Goal, Enemies[numEnemies];  // Player and Goal positions

pos RandSpace() {  // Find random space
	pos Rand;
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
		pos Rand = RandSpace();
		map[Rand.y][Rand.x] = 'G';
		break;
	}
}

void UpdateEnemies() {  // Enemy movement
	for (int i = 0; i < numEnemies; i++) {
		if (rand() % enemySpeed == 0) {  // Decide to move
			int direction = rand() % 4;  // Decide direction
			map[Enemies[i].y][Enemies[i].x] = '.';  // Remove enemy
			switch (direction) {
			case 0:  // Up
				Enemies[i].y--;  // Move enemy
				if (map[Enemies[i].y][Enemies[i].x] == 'W' || map[Enemies[i].y][Enemies[i].x] == 'G')  // If wall or goal
					Enemies[i].y++;  // Move back
				break;
			case 1:  // Down
				Enemies[i].y++;
				if (map[Enemies[i].y][Enemies[i].x] == 'W' || map[Enemies[i].y][Enemies[i].x] == 'G')
					Enemies[i].y--;
				break;
			case 2:  // Left
				Enemies[i].x--;
				if (map[Enemies[i].y][Enemies[i].x] == 'W' || map[Enemies[i].y][Enemies[i].x] == 'G')
					Enemies[i].x++;
				break;
			case 3:  // Right
				Enemies[i].x++;
				if (map[Enemies[i].y][Enemies[i].x] == 'W' || map[Enemies[i].y][Enemies[i].x] == 'G')
					Enemies[i].x--;
				break;
			}

			if (map[Enemies[i].y][Enemies[i].x] == 'P') {  // Check if enemy hit player
				cout << "An enemy got you! You lose. (Score: " << GetElapsedTime() << ")" << endl;
				Player.x = kMazeX + 1;
				Player.y = kMazeY + 1;
			}
			map[Enemies[i].y][Enemies[i].x] = 'E';
		}
	}
}

void MovePlayer(EKeyPressed key) {  // Player movement
	if (key == EKeyPressed::eNone || Player.x > kMazeX && Player.y > kMazeY) { // If none or dead do nothing
		return;
	}
	map[Player.y][Player.x] = '.';  // Remove player

	switch (key) {
	case EKeyPressed::eUp:  // Key pressed
		Player.y--;  // Move player
		if (map[Player.y][Player.x] == 'W')  // If wall move back
			Player.y++;
		break;
	case EKeyPressed::eDown:
		Player.y++;
		if (map[Player.y][Player.x] == 'W')
			Player.y--;
		break;
	case EKeyPressed::eLeft:
		Player.x--;
		if (map[Player.y][Player.x] == 'W')
			Player.x++;
		break;
	case EKeyPressed::eRight:
		Player.x++;
		if (map[Player.y][Player.x] == 'W')
			Player.x--;
		break;
	}

	if (map[Player.y][Player.x] == 'G') {  // Check if player hit goal
		cout << "You found goal number " << numGoals++ << " (Score: " << GetElapsedTime() << ")" << endl;
		NewGoal();  // Generate new goal
		StartClock(); // Restart timer for next score
	}
	else if (map[Player.y][Player.x] == 'E') {  // Check if player hit enemy
		cout << "An enemy got you! You lose. (Score: " << GetElapsedTime() << ")" << endl;
		Player.x = kMazeX + 1;
		Player.y = kMazeY + 1;
	}
	map[Player.y][Player.x] = 'P';  // Move player
}

void DrawMaze() {  // Draw the maze
	for (int y = 0; y < kMazeY; y++) {  // Loop through entire maze
		for (int x = 0; x < kMazeX; x++) {
			switch (map[y][x]) {
			case 'W':  // Wall
				ChangeColour(100, 100, 100);  // Grey
				DrawRectangle(x * cellWidth, y * cellHeight, cellWidth, cellHeight);
				break;
			case 'G':  // Goal
				ChangeColour(50, 255, 50);  // Green
				DrawRectangle(x * cellWidth, y * cellHeight, cellWidth, cellHeight);
				break;
			case 'P':  // Player
				ChangeColour(255, 255, 50);  // Yellow
				DrawCircle(x * cellWidth + (radius / 2), y * cellHeight + (radius / 2), radius);
				break;
			case 'E':  // Enemy
				ChangeColour(255, 50, 50);  // Red
				DrawCircle(x * cellWidth + (radius / 2), y * cellHeight + (radius / 2), radius);
				break;
			default:  // Other
				break;
			}
		}
	}
}

int main()
{
	srand(time(NULL));  // Set random seed
	
	Player = RandSpace();  // Random player start
	map[Player.y][Player.x] = 'P';
	
	Goal = RandSpace();  // Random goal start
	map[Goal.y][Goal.x] = 'G';

	for (int i = 0; i < numEnemies; i++) {
		Enemies[i] = RandSpace();  // Random enemy start
		map[Enemies[i].y][Enemies[i].x] = 'E';
	}

	StartClock();  // Start score timer
	while (UpdateFramework())
	{

		MovePlayer(GetLastKeyPressed());
		UpdateEnemies();
		DrawMaze();
	}

	return 0;
}
