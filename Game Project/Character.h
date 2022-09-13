#pragma once

#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

struct Upgrades { //0-5 increase by one for each upgrade
	int health; // 20 each
	int armor; // 2 each
	int attack; // 5 each
	int evade; // 2 each
	int crit; // 4 each
	int essence; //storing it here because idk where else to
};

struct Effect {
	string name;
	string type;// "AB" = attack buff, "AD" = attack debuff, "DB" = defense buff, "DD" = defense debuff, "SB" = special buff, "SD" = special debuff
	double amount;
	int duration;
	string message;
};

struct Skill { // my god this is a headache to handle 
	bool unlocked;
	string name; 
	string message;
	string description;
	int damage;
	bool isAOE;
	bool isBuff;
	bool isDebuff;
	Effect effect;
	int cost;
};


class Character {
public:
	Character();
	Character(string name, int health, int armor, int crystals, int crystalsR, int evadeChance,	int critChance,	int critMultiplier, int attackV, string attackMessage, string defendMessage, string chargeMessage, int numOfSkills);
	Character(ifstream&);
	virtual ~Character();
	

	string getName() { return name; }
	void setName(string n) { name = n; }
	int getMaxHealth() { return maxHealth; }
	void setMaxHealth(int newMax) { maxHealth = newMax; }
	int getCurrentHealth() { return currentHealth; }
	void setCurrentHealth(int newHealth) { currentHealth = newHealth; }
	void decreaseCurrentHealth(int decrease);
	void increaseCurrentHealth(int increase);

	int getArmor() { return armor; }
	void setArmor(int newArmor);
	double getDamageReduction() { return damageReduction; }

	int getMaxCrystals() { return maxCrystals; }
	void setMaxCrystals(int newMax) { maxCrystals = newMax; }
	int getCrystalRegen() { return crystalRegen; }
	void setCrystalregen(int newReg) { crystalRegen = newReg; }
	int getCurrentCrystals() { return currentCrystals; }
	void setCurrentCrystals(int current) { currentCrystals = current; }
	void decreaseCurrentCrystals(int decrease) { currentCrystals -= decrease; }
	void regenCrystals();

	int getEvadeChance() { return evadeChance; }
	void setEvadeChance(int evade) { evadeChance = evade; }
	int getCritChance() { return critChance; }
	void setCritChance(int critC) { critChance = critC; }
	int getCritMultiplier() { return critMultiplier; }
	void setCritMultiplier(int critM) { critMultiplier = critM; }
	int getAttackValue() { return attackValue; }
	void setAttackValue(int attackV) { attackValue = attackV; }

	void setAttackMessage(string attack) { attackMessage = attack; }
	void setDefendMessage(string defend) { defendMessage = defend; }
	void setChargeMessage(string charge) { chargeMessage = charge; }

	void addBuff(Effect buff) { buffs.push_back(buff); }
	Effect getBuffAt(int i) { return buffs[i]; }
	void removeBuff(int i) { buffs.erase(buffs.begin() + i); }
	bool hasBuff(string buff);
	int getNumberOfBuffs() { return buffs.size(); }
	void addDebuff(Effect buff) { debuffs.push_back(buff); }
	Effect getDebuffAt(int i) { return debuffs[i]; }
	void removeDebuff(int i) { debuffs.erase(debuffs.begin() + i); }
	bool hasDebuff(string debuff);
	int getNumberOfDebuffs() { return debuffs.size(); }
	void decreaseAllEffectDurations();

	Skill getSkill(int i) { return allSkills[i]; }
	void setSkill(Skill newSkill, int i) { allSkills[i] = newSkill; }
	void unlockSkill(int i) { allSkills[i].unlocked = true; }
	Skill getStoredSkill(int i) { return skills[i]; }
	void setStoredSkil(Skill newSkill, int i) { skills[i] = newSkill; }
	void displaySkills();

	int getGold() { return gold; }
	void setGold(int g) { gold = g; }
	void increaseGold(int g) { gold += g; }
	void decreaseGold(int g) { gold -= g; }
	int getEssence() { return essence; }
	void setEssence(int e) { essence = e; }
	void increaseEssence(int e) { essence += e; }
	void decreaseEssence(int e) { essence -= e; }

	void applyUpgrades(Upgrades u);
	void displayStatus();

	//wasted so much time trying to make these pure virtual fml
	virtual int attack();
	virtual Skill useSkill(int);
	virtual void defend();
	virtual void charge();

	virtual void specialAbility() = 0;



protected:
	//stats and stuff
	string name;
	int maxHealth;
	int currentHealth;
	int armor;
	double damageReduction;
	int maxCrystals;
	int crystalRegen;
	int currentCrystals;
	int attackValue;
	int evadeChance;
	int critChance;
	int critMultiplier;
	string attackMessage;
	string defendMessage;
	string chargeMessage;

	//currencies
	int gold = 0;
	int essence = 0;

	vector<Effect> buffs;
	vector<Effect> debuffs;
	Skill *allSkills;
	Skill skills[4]; //skills stored in skill slots
	void calculateDR();
};

#endif#pragma once
