#pragma once
#ifndef MAP_H
#define MAP_H
#include "Battle.h"
#include "Character.h"
#include "Enemy.h"
using namespace std;

class Map
{
public:
	Map(Character*);
	int nextLocation(); //return 0 for a battle, return 1 for a campsite, return 2 for boss battle, 
	Battle generateBattle();
	Battle generateBossBattle();
	void buffRandomStat(Enemy*,int);
	void campsite();

	int getLocationCounter() { return locationCounter; }
private:
	int locationCounter;
	Character* player;
	Enemy* enemies;
	
};

#endif

