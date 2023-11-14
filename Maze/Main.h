#ifndef MAIN_H
#define MAIN_H
struct Pos {
	int x = 0, y = 0;
};

Pos RandSpace();
void NewGoal();

class Player {
public:
	Pos pos;
	Player();
	void Move(EKeyPressed key);
};

class Enemy : public Player {
public:
	void Move(Player& player);
};

#endif
