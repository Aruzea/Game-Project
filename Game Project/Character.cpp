#include "Character.h"
#include <iomanip>
using namespace std;

Character::Character() : name(""),maxHealth(0),currentHealth(0),armor(0),damageReduction(0),maxCrystals(0),crystalRegen(0),currentCrystals(0),attackValue(0) {
	allSkills = new Skill[1];
}
Character::Character(string n, int health, int arm, int crystals, int crystalsR, int evadeC, int critC, int critM, int attackV, string attackM, string defendM, string chargeM, int numOfSkills) 
	: name(n),maxHealth(health),currentHealth(health), armor(arm),maxCrystals(crystals),currentCrystals(1),crystalRegen(crystalsR), evadeChance(evadeC), critChance(critC), critMultiplier(critM), attackValue(attackV), attackMessage(attackM), defendMessage(defendM),chargeMessage(chargeM)
{
	allSkills = new Skill[numOfSkills];
	calculateDR();
}

Character::Character(ifstream &file) { //help me
	int health, armor, maxCrystals, crystalR, evadeC, critC, critM, attackValue, numSkills;
	string name, attackM, defendM, chargeM;
	file >> name;
	this->name = name;
	file >> health;
	this->currentHealth = health;
	this->maxHealth = health;
	file >> armor;
	this->armor = armor;
	file >> maxCrystals;
	this->maxCrystals = maxCrystals;
	this->currentCrystals = 1;
	file >> crystalR;
	this->crystalRegen = crystalR;
	file >> evadeC;
	this->evadeChance = evadeC;
	file >> critC;
	this->critChance = critC;
	file >> critM;
	this->critMultiplier = critM;
	file >> attackValue;
	this->attackValue = attackValue;
	file.ignore();
	getline(file, attackM);
	this->attackMessage = attackM;
	getline(file, defendM);
	this->defendMessage = defendM;
	getline(file, chargeM);
	this->chargeMessage = chargeM;

	string sRead;
	int iRead;
	double dRead;
	bool bRead;
	Effect effect;
	
	file >> numSkills;
	allSkills = new Skill[numSkills];
	for (int i = 0; i < numSkills; i++) {
		sRead = "";
		iRead = -1;
		dRead = -1;
		bRead = 0;
		Effect effect;
		file >> bRead;
		allSkills[i].unlocked = bRead;
		file.ignore();
		getline(file, sRead);
		allSkills[i].name = sRead;
		getline(file, sRead);
		allSkills[i].message = sRead;
		getline(file, sRead);
		allSkills[i].description = sRead;
		file >> iRead;
		allSkills[i].damage = iRead;
		file >> bRead;
		allSkills[i].isAOE = bRead;
		file >> bRead;
		allSkills[i].isBuff = bRead;
		file >> bRead;
		allSkills[i].isDebuff = bRead;
		file >> sRead;
		effect.name = sRead;
		if (sRead != "none") {
			file >> sRead;
			effect.type = sRead;
			file >> dRead;
			effect.amount = dRead;
			file >> iRead;
			effect.duration = iRead + 1;
			file.ignore();
			getline(file, sRead);
			effect.message = sRead;
		}
		allSkills[i].effect = effect;
		file >> iRead;
		allSkills[i].cost = iRead;
	}
	
	for (int i = 0; i < 4; i++) {
		skills[i] = allSkills[i];
	}
}

Character::~Character() {
	delete[] allSkills;
}

void Character::setArmor(int newArmor) {
	armor = newArmor;
	calculateDR();
}

void Character::calculateDR() {
	damageReduction = ((90. * armor) / (armor + 37.))/100;
}

void Character::decreaseCurrentHealth(int decrease) {
	if (decrease > currentHealth) {
		currentHealth = 0;
	}
	else
		currentHealth -= decrease;
}

void Character::increaseCurrentHealth(int increase) {
	if (increase + currentHealth > maxHealth) {
		currentHealth = maxHealth;
	}
	else
		currentHealth += increase;
}

void Character::regenCrystals() {
	if (crystalRegen + currentCrystals > maxCrystals)
		currentCrystals = maxCrystals;
	else
		currentCrystals += crystalRegen;
}

bool Character::hasBuff(string name) {
	for (int i = 0; i < buffs.size(); i++) {
		if (buffs[i].name == name) {
			return true;
		}
	}
	return false;
}

bool Character::hasDebuff(string name) {
	for (int i = 0; i < debuffs.size(); i++) {
		if (debuffs[i].name == name) {
			return true;
		}
	}
	return false;
}

int Character::attack() {
	cout << name << attackMessage << endl;;
	return attackValue;
}
Skill Character::useSkill(int index) { // come up with a a better idea you idiot
	return skills[index]; //THIS IS SUCH A BAD SOLUTION YOU DAMN IDIOT
} // I hate this function

void Character::defend() {
	cout << name << defendMessage << endl;
	Effect defending;
	defending.name = "defending";
	defending.type = "DB";
	defending.duration = 0;
	defending.amount = .50;
	buffs.push_back(defending);
}
void Character::charge() {
	cout << name << chargeMessage << endl;
	if (crystalRegen + currentCrystals + 1 > maxCrystals)
		currentCrystals = maxCrystals;
	else
		currentCrystals += crystalRegen + 1;
}

void Character::displaySkills() {
	for (int i = 0; i < 4; i++) {
		cout << i+1 << ". " << skills[i].name << "(Cost: " << skills[i].cost << ") - " << skills[i].description << endl;;
	}
}

void Character::decreaseAllEffectDurations() {
	for (int i = 0; i < buffs.size(); i++) {
		if(buffs[i].duration > 0)
			buffs[i].duration--;
		if (buffs[i].duration == 0)
			buffs.erase(buffs.begin() + i);
	}
	for (int i = 0; i < debuffs.size(); i++) {
		if (debuffs[i].duration > 0)
			debuffs[i].duration--;
		if (debuffs[i].duration == 0)
			debuffs.erase(debuffs.begin() + i);
	}
}

void Character::applyUpgrades(Upgrades u) {
	maxHealth += u.health * 20;
	currentHealth = maxHealth;
	armor += u.armor * 2;
	calculateDR();
	attackValue += u.attack * 5;
	evadeChance += u.evade * 2;
	critChance += u.crit * 4;
}

void Character::displayStatus() {
	cout << "Status:" << endl;
	cout << "Max Health: " << maxHealth << endl;
	cout << "Armor: " << armor << " (" << fixed << setprecision(2) << damageReduction << ")" << endl;
	cout << "Max Crystals: " << maxCrystals << endl;
	cout << "Crystal Regen: " << crystalRegen << endl;
	cout << "Basic Attack Damage: " << attackValue << endl;
	cout << "Evade Chance: " << evadeChance << "%" << endl;
	cout << "Critical Chance: " << critChance << "%" << endl;
	cout << "Critical Multiplier: " << critMultiplier << "x" << endl;
}