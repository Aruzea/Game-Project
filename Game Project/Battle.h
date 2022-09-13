#pragma once

#ifndef BATTLE_H
#define BATTLE_H
#include "Character.h"
#include "Berserker.h"
#include "Enemy.h"
using namespace std;


class Battle {
public:
	Battle() {}
	Battle(Character* thePlayer, Enemy** theEnemies,int numE);
	void executeBattle();//THERE SHALL BE BLOOD
	~Battle();
	

private:
	Character *player;
	Enemy **enemies;
	int numEnemies;
	double compileEffects(Character*, string type); //gets the sum of buffs/debuffs on a player
	void damage(Character*, Character*, int damage);
	void displayUI();
	void playerUI();
	void enemyUI();
	bool healthCheck();
	void executePlayerAction(int, int, int, Skill);
	void executeEnemyActions(int enemyChoices[]);
	void durationTick(); //lowers duration of all effects by one
	int getPlayerAttackChoice();
	bool isValidAttack(int);

	bool isFineToDelete = 0;



};

#endif