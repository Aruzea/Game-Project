#include "Berserker.h"
#include <iostream>
using namespace std;


Berserker::Berserker(string n, int health, int arm, int crystals, int crystalsR, int evadeC, int critC, int critM, int attackV, string attackM, string defendM, string chargeM, int numOfSkills)
	: Character(n, health, arm, crystals, crystalsR, evadeC, critC, critM, attackV, attackM, defendM, chargeM, numOfSkills), rageGauge(99), rageActive(false) {}

Berserker::Berserker(ifstream file) : Character(file) {
	rageGauge = 0;
	rageActive = false;
	file.close();
}

void Berserker::specialAbility() {
	if (!rageActive) {
		//cout << round(((double)(maxHealth - currentHealth) / (double)maxHealth) * 100) << endl;
		increaseRageGauge(round(((double)(maxHealth - currentHealth) / (double)maxHealth) * 100));
		cout << "Rage: " << rageGauge << "%" << endl;
		if (rageGauge >= 100) {
			cout << "Your rage boils over! You are now enraged. (3 turns)." << endl;
			rageActive = true;
			Effect enragedB;
			enragedB.name = "enraged";
			enragedB.type = "SB";
			enragedB.duration = 4;
			enragedB.message = "Your limiters have been released. +100% attack damage and +50% lifesteal.";
			this->addBuff(enragedB);
			Effect enragedD;
			enragedD.name = "enraged";
			enragedD.type = "SD";
			enragedD.duration = 4;
			enragedD.message = "You have lost all reason. You automatically attack the enemy.";
			this->addDebuff(enragedD);
			rageGauge = 0;
		}
	}
	else
		if (!this->hasBuff("enraged"))
			rageActive = false;

}


