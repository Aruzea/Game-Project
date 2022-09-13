#pragma once

#ifndef BERSERKER_H
#define BERSERKER_H
#include "Character.h"
#include <fstream>
using namespace std;

class Berserker : public Character {
public: 
	Berserker(string n, int health, int arm, int crystals, int crystalsR, int evadeC, int critC, int critM, int attackV, string attackM, string defendM, string chargeM, int numOfSkills);
	Berserker(ifstream file);
	void specialAbility() override;

	void increaseRageGauge(int rage) { rageGauge += rage; }

private:
	bool rageActive;
	int rageGauge;
};

#endif