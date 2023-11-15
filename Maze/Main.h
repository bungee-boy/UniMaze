#ifndef MAIN_H
#define MAIN_H

const char M_PLAYER{ 'P' }, M_ENEMY{ 'E' }, M_WALL{ 'W' }, M_GOAL{ 'G' }, M_SPACE{ '.' };  // Map characters
struct Pos {
	int x = 0, y = 0;
};

Pos RandSpace();
void NewGoal();

class Player {
public:
	Pos pos;
	char map_block[1]{ M_WALL };  // Blocked map movements
	char map_value{ M_PLAYER };  // Map identifier
	Player();
	bool IsAllowed();
	void Move(EKeyPressed key);
};

class Enemy : public Player {
public:
	Enemy();
};

#endif
