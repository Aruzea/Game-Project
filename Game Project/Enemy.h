#pragma once
#ifndef ENEMY_H
#define ENEMY_H
#include <vector>
#include <string>
#include <iostream>
#include "Character.h"
using namespace std;

class Enemy : public Character { //im being desperate aaaaaaa
public:
	Enemy();
	Enemy(string n, int health, int arm, int crystals, int crystalsR, int evadeC, int critC, int critM, int attackV, string attackM, string defendM, string chargeM, string enemyType);
	Enemy(ifstream file);
	Enemy(const Enemy& another);
	void specialAbility() override;
	int makeChoice(int playerHealth, int playerCrystals);
	void setEnemyType(string type) { enemyType = type; }
	Skill whichSkill();

private:
	string enemyType;
	int makeChoice(int, int, int, int);
};

#endif
