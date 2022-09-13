#include "Enemy.h"
#include <iostream>
#include <time.h>

void Enemy::specialAbility() {
	if (enemyType == "boss_wolf") {
		if (currentHealth < round(maxHealth * .33)) {
			cout << name << " is using its lifeforce to empower itself for a final stand!" << endl;
			attackValue += 20;
			evadeChance += 10;
			critChance += 10;
			critMultiplier++;
			enemyType = "empowered_boss_wolf";
		}
	}

}

Enemy::Enemy() : Character() {}

Enemy::Enemy(string n, int health, int arm, int crystals, int crystalsR, int evadeC, int critC, int critM, int attackV, string attackM, string defendM, string chargeM, string type)
	: Character(n, health, arm, crystals, crystalsR, evadeC, critC, critM, attackV, attackM, defendM, chargeM, 4), enemyType(type) {}

Enemy::Enemy(const Enemy& another) {
	this->name = another.name;
	this->maxHealth = another.maxHealth;
	this->currentHealth = another.currentHealth;
	this->armor = another.armor;
	this->currentCrystals = another.currentCrystals;
	this->maxCrystals = another.maxCrystals;
	this->crystalRegen = another.crystalRegen;
	this->evadeChance = another.evadeChance;
	this->critChance = another.critChance;
	this->critMultiplier = another.critMultiplier;
	this->attackValue = another.attackValue;;
	this->attackMessage = another.attackMessage;
	this->defendMessage = another.defendMessage;
	this->chargeMessage = another.chargeMessage;
	this->enemyType = another.enemyType;
	calculateDR();
	for (int i = 0; i < 4; i++) {
		this->skills[i] = another.skills[i];
	}
}

Enemy::Enemy(ifstream file) : Character(file) {
	string type;
	file.ignore();
	file >> type;
	enemyType = type;
	file.close();
}

int Enemy::makeChoice(int playerHealth, int playerCrystals) { //I was intending to make enemies make choices based off player conditions but me dumb and me have no time
	int attackChance = 0;
	int skillChance = 0;
	int defendChance = 0;
	int chargeChance = 0;
	if (enemyType == "wolf" || enemyType == "boss_wolf") { //imagine having more than one enemy type hahaha
		attackChance = 60;
		defendChance = 25;
		skillChance = currentCrystals * 10;
		attackChance -= skillChance;
		chargeChance = 15;
	}
	else if (enemyType == "empowered_boss_wolf") {
		attackChance = 100;
		defendChance = 0;
		skillChance = currentCrystals * 20;
		attackChance -= skillChance;
		chargeChance = 0;
	}
	return makeChoice(attackChance, skillChance, defendChance, chargeChance);
}

Skill Enemy::whichSkill() {
	return skills[rand() % currentCrystals];
}

int Enemy::makeChoice(int attack, int skill, int defend, int charge) {
	int chances[4] = { attack,skill,defend,charge };
	for (int i = 0; i < 4; i++) {
		if (chances[i] == 0) continue;
		chances[i] = rand() % chances[i];
	}

	int greatestIndex = 0;
	for (int i = 0; i < 4; i++) {
		if (chances[i] > chances[greatestIndex])
			greatestIndex = i;
	}
	return greatestIndex + 1;
}