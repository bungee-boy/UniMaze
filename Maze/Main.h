#ifndef MAIN_H
#define MAIN_H

const char M_PLAYER{ 'P' }, M_ENEMY{ 'E' }, M_WALL{ 'W' }, M_GOAL{ 'G' }, M_SPACE{ '.' };  // Map characters
const int M_SIZE_X{ 20 }, M_SIZE_Y{ 20 };

struct Pos {
	int x = 0, y = 0;
};

Pos RandSpace();
void NewGoal();

class Map {
public:
	std::string fileName{ "map_1.txt" };
	char currLevel[M_SIZE_X][M_SIZE_Y]{
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
		{ 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W' }};
	Map();
	void Save();
	void Load();
	void Draw();
	void Update(EKeyPressed key);
};

class Player {
public:
	Pos pos;
	char map_block[1]{ M_WALL };  // Blocked map movements
	char map_value{ M_PLAYER };  // Map identifier
	Player();
	bool IsAllowed();
	void Move(EKeyPressed key);
};

class Enemy : public Player {  // inherits and uses polymorphism from Player
public:
	Enemy();
};

#endif
