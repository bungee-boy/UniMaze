/*
	GPF Week 6 - Maze Start Code
*/

#include <iostream>
#include <vector>
#include <time.h>
#include <stdio.h>

#include "Framework.h"
using namespace std;

// Screen dimensions
int gScreenWidth{ 800 };
int gScreenHeight{ 800 };

// Delay to slow things down
int gTimeDelayMS{ 100 };

// Maze size as constants
constexpr int kMazeX{ 20 };
constexpr int kMazeY{ 20 };

/*
	2 Dimensional Arrays
	You can think of these as being an array of arrays
	
	The maze has kMazeRowsY columns across (20 by default) represented by an array
	We then have kMazeColumnsX (20) of these arrays, one for each row
	
	Columns and rows can be confusing so we tend to prefer working with x and y
	x is the horizontal axis (columns) across and y is the vertical axis (rows) down.

	Each single item is called a cell. 
	
	E.g. to output the value of the cell at 16 X (column 16) and 1 Y (row 1) we would write:
	cout << map[1][16] << endl; // outputs G
*/

char map[kMazeX][kMazeY] = {
	// 20 columns (x axis) by 20 rows (y axis)
	// X0   X1   X2   X3   X4   X5   X6   X7   X8   X9   X10  X11  X12  X13  X14  X15  X16  X17  X18  X19 
	{ 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W' },	//Y0
	{ 'W', '.', '.', '.', '.', 'W', '.', '.', '.', 'W', 'W', '.', '.', '.', '.', 'W', '.', '.', '.', 'W' },	//Y1
	{ 'W', '.', 'W', 'W', 'W', 'W', 'W', 'W', '.', 'W', '.', '.', 'W', 'W', 'W', 'W', 'W', 'W', '.', 'W' },	//Y2
	{ 'W', '.', 'W', '.', '.', '.', 'W', '.', '.', 'W', '.', 'W', 'W', '.', 'W', '.', '.', '.', '.', 'W' },	//Y3
	{ 'W', '.', 'W', '.', 'W', '.', 'W', '.', 'W', 'W', '.', 'W', 'W', '.', 'W', 'W', 'W', '.', 'W', 'W' },	//Y4
	{ 'W', '.', 'W', '.', 'W', '.', '.', '.', '.', '.', '.', '.', 'W', '.', 'W', '.', '.', '.', '.', 'W' },	//Y5
	{ 'W', '.', 'W', '.', 'W', 'W', 'W', '.', '.', 'W', 'W', '.', 'W', '.', '.', '.', 'W', '.', '.', 'W' },	//Y6
	{ 'W', '.', 'W', '.', 'W', '.', 'W', 'W', 'W', 'W', 'W', '.', 'W', '.', 'W', '.', 'W', 'W', 'W', 'W' },	//Y7
	{ 'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W', '.', '.', '.', 'W', '.', '.', '.', '.', 'W' },	//Y8
	{ 'W', '.', 'W', '.', 'W', '.', 'W', 'W', '.', 'W', 'W', 'W', '.', 'W', 'W', 'W', 'W', '.', 'W', 'W' },	//Y9
	{ 'W', '.', 'W', 'W', 'W', 'W', 'W', 'W', '.', 'W', 'W', 'W', '.', 'W', '.', 'W', 'W', '.', 'W', 'W' },	//Y10
	{ 'W', '.', '.', '.', '.', 'W', '.', '.', '.', 'W', 'W', '.', '.', '.', '.', 'W', '.', '.', '.', 'W' },	//Y11
	{ 'W', '.', 'W', 'W', 'W', 'W', 'W', 'W', '.', '.', '.', '.', 'W', '.', 'W', 'W', 'W', 'W', '.', 'W' },	//Y12
	{ 'W', '.', 'W', '.', '.', '.', 'W', '.', '.', 'W', 'W', '.', 'W', '.', 'W', '.', '.', '.', '.', 'W' },	//Y13
	{ 'W', '.', 'W', '.', 'W', '.', 'W', '.', 'W', 'W', 'W', '.', 'W', '.', 'W', '.', 'W', 'W', 'W', 'W' },	//Y14
	{ 'W', '.', 'W', '.', 'W', '.', '.', '.', '.', '.', 'W', '.', 'W', '.', 'W', '.', '.', '.', 'W', 'W' },	//Y15
	{ 'W', '.', 'W', '.', 'W', 'W', 'W', '.', '.', 'W', 'W', '.', 'W', '.', '.', '.', 'W', '.', '.', 'W' },	//Y16
	{ 'W', '.', 'W', '.', 'W', '.', 'W', 'W', 'W', 'W', '.', '.', 'W', 'W', 'W', '.', 'W', 'W', '.', 'W' },	//Y17
	{ 'W', 'P', '.', '.', '.', '.', '.', '.', '.', 'W', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'W' },	//Y18
	{ 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W' },	//Y19
};

const int cellWidth = gScreenWidth / kMazeX;
const int cellHeight = gScreenHeight / kMazeY;
int playerX = 0;
int playerY = 0;
int numGoals = 1;

void GetPlayer() {  // Get player postition
	for (int y = 0; y < kMazeY; y++) {
		for (int x = 0; x < kMazeX; x++) {
			if (map[y][x] == 'P') {
				playerX = x;
				playerY = y;
				return;
			}
		}
	}
}

void NewGoal() {  // Randomise new goal
	int randX;
	int randY;
	while (true) {
		randY = rand() % kMazeY;
		randX = rand() % kMazeX;
		if (map[randY][randX] == '.') {
			map[randY][randX] = 'G';
			break;
		}
	}
	cout << "Find the new goal!" << endl;
}

void CheckGoal(int posX, int posY) {  // Check if x, y is a goal
	if (map[posY][posX] == 'G') {
		cout << "You found goal number " << numGoals++ << endl;
		NewGoal();
	}
}

void MovePlayer(EKeyPressed key) {  // Player movement
	if (key == EKeyPressed::eUp) {
		if (map[playerY - 1][playerX] != 'W') {
			map[playerY][playerX] = '.';
			playerY--;
			CheckGoal(playerX, playerY);
			map[playerY][playerX] = 'P';
		}
	}
	else if (key == EKeyPressed::eDown) {
		if (map[playerY + 1][playerX] != 'W') {
			map[playerY][playerX] = '.';
			playerY++;
			CheckGoal(playerX, playerY);
			map[playerY][playerX] = 'P';
		}
	}
	else if (key == EKeyPressed::eLeft) {
		if (map[playerY][playerX - 1] != 'W') {
			map[playerY][playerX] = '.';
			playerX--;
			CheckGoal(playerX, playerY);
			map[playerY][playerX] = 'P';
		}
	}
	else if (key == EKeyPressed::eRight) {
		if (map[playerY][playerX + 1] != 'W') {
			map[playerY][playerX] = '.';
			playerX++;
			CheckGoal(playerX, playerY);
			map[playerY][playerX] = 'P';
		}
	}
}

void DrawMaze() {  // Draw the maze
	for (int y = 0; y < kMazeY; y++) {
		for (int x = 0; x < kMazeX; x++) {
			switch (map[y][x]) {
			case 'W':
				ChangeColour(100, 100, 100);  // Grey
				DrawRectangle(x * cellWidth, y * cellHeight, cellWidth, cellHeight);
				break;
			case 'G':
				ChangeColour(50, 255, 50);  // Green
				DrawRectangle(x * cellWidth, y * cellHeight, cellWidth, cellHeight);
				break;
			case 'P': {
				int radius = (cellWidth > cellHeight ? cellWidth : cellHeight) / 3;
				ChangeColour(255, 255, 50);  // Yellow
				DrawCircle(x * cellWidth + (radius / 2), y * cellHeight + (radius / 2), radius);
				break;}
			default:
				break;
			}
		}
	}
}

int main()
{
	srand(time(NULL));  // Set random seed
	GetPlayer();
	NewGoal();  // Generate goal position on start
	while (UpdateFramework())
	{
		MovePlayer(GetLastKeyPressed());
		DrawMaze();
	}

	return 0;
}
