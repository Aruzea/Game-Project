#include "Battle.h"
#include <iostream>
#include <time.h>
#include <Windows.h>
using namespace std; 
//bunch of Sleep(1000) to pause between lines and also allows srand(time(NULL)) to actually be a different seed each time

Battle::Battle(Character* thePlayer, Enemy** theEnemies,int num) {
	player = thePlayer;
	enemies = theEnemies;
	numEnemies = num;
}

Battle::~Battle() {
	if (isFineToDelete) { //for some reason it deletes at the same time map generates so DUCT TAPE FIX IT IS
		for (int i = 0; i < numEnemies; i++)
			delete enemies[i];
		delete[]enemies;
	}
}

void Battle::damage(Character* attacker, Character* defender, int damage){ // I REALLY hope there are no problems here, math dumb
	if (rand() % 100 + 1 < defender->getEvadeChance()) {
		cout << defender->getName() << " evades the attack!\n" << endl;
		Sleep(1000);
		return;
	}
	
	double attackMultiplier = compileEffects(attacker, "AB") - compileEffects(attacker, "AD");
	double defenseMultiplier = defender->getDamageReduction() + compileEffects(defender, "DB") - compileEffects(defender, "DD");
	damage += round(damage * attackMultiplier);

	if (rand() % 100 + 1 < attacker->getCritChance()) {
		damage *= attacker->getCritMultiplier();
		cout << "The attack crits!" << endl;
	}
	damage -= round(damage * defenseMultiplier);
	cout << defender->getName() << " takes " << damage << " damage!" << endl;
	defender->decreaseCurrentHealth(damage);
	Sleep(1000);
	
	if (attacker->hasBuff("enraged")) {//I wish I was smart enough to find a better way to do this (this is the lifesteal btw)
		attacker->increaseCurrentHealth(round(damage * .50));
		cout << "Berserker heals " << round(damage * .50) << " health." << endl;
		Sleep(1000);
	}
	cout << endl;
}

double Battle::compileEffects(Character* entity, string type) { 
	double totalMultiplier = 0;
	if (type == "AB" || type == "DB") {
		for (int i = 0; i < entity->getNumberOfBuffs(); i++) {
			if (entity->getBuffAt(i).type == type)
				totalMultiplier += entity->getBuffAt(i).amount;
		}
	}
	else {
		for (int i = 0; i < entity->getNumberOfDebuffs(); i++) {
			if (entity->getDebuffAt(i).type == type)
				totalMultiplier += entity->getDebuffAt(i).amount;
		}
	}
	return totalMultiplier;
}

void Battle::executeBattle() { //probaly made this function too long. whoopsies
	int* enemyChoices = new int[numEnemies];
	while (healthCheck()) {
		int playerChoice = -1;
		int playerSkillChoice = -1;
		int playerAttackChoice = -1;
		Skill playerSkill;
		Skill enemySkill;
		
		while(true) { //probably terrible implementation
			displayUI();
			if (player->hasBuff("enraged")) {
				playerChoice = 1;
				srand(time(NULL));
				playerAttackChoice = rand() % numEnemies + 1;
				if(!isValidAttack(playerAttackChoice))
					for (int i = 0; i < numEnemies; i++) {
						if (isValidAttack(i)) playerAttackChoice = i;
					}
			}
			else {
				cin >> playerChoice;
				cin.ignore();
				if (playerChoice == 1) {
					playerAttackChoice = this->getPlayerAttackChoice();
					if (playerAttackChoice == 0) continue;
				}
			}
			if (playerChoice == 2) {
				cout << "Which skill would you like to use? Enter 0 to return." << endl;
				player->displaySkills();
				cout << "Skill: ";
				cin >> playerSkillChoice;
				cin.ignore();
				if (playerSkillChoice == 0) continue;
				playerSkill = player->useSkill(playerSkillChoice-1);

				if(playerSkill.cost > player->getCurrentCrystals()){
					cout << "Not enough crystals." << endl;
					cout << "Press enter to continue...";
					cin.ignore();
					continue;
				}

				if (!playerSkill.isAOE && !playerSkill.isBuff) {
					playerAttackChoice = getPlayerAttackChoice();
					if (playerAttackChoice == 0) continue;
				}
			}
			break;
		}

		for (int i = 0; i < numEnemies; i++) {
			enemyChoices[i] = enemies[i]->makeChoice(player->getCurrentHealth(), player->getCurrentCrystals());
		}

		for (int i = 0; i < numEnemies; i++) //show which enemies are defending first to avoid confusion 
			if (enemyChoices[i] == 3) {
				enemies[i]->defend();
				enemies[i]->regenCrystals();
				cout << endl;
				Sleep(1000);
			}

		//player action
		this->executePlayerAction(playerChoice, playerAttackChoice - 1, playerSkillChoice, playerSkill);

		if (!healthCheck()) break; // making sure the enemies are not dead
		
		//enemy action
		executeEnemyActions(enemyChoices);
		
		if (!healthCheck()) break;

		player->specialAbility();
		durationTick();

		cout << "\nPress enter to continue...";
		cin.ignore();
		
	}
	delete[]enemyChoices;
	isFineToDelete = true;
	for (int i = 0; i < 10; i++) durationTick(); //lazy way to purge buffs, i know
}

void Battle::displayUI() { //shows all the fancy stuff
	for (int i = 0; i < 100; i++) cout << endl; //clears the screen

	//enemy stuff
	enemyUI();
	
	cout << endl;
	cout << "-------------------------------------------------------------------------------------------" << endl;

	//player stuff
	playerUI();

	if (!player->hasBuff("enraged")) { //skips choices if enraged
		cout << endl;
		cout << "Enter your choice: " << endl;
		cout << "1. Attack" << "\t2. Use Skill" << endl << "3. Defend" << "\t4. Charge\n" << endl << "Choice: ";
	}
}

void Battle::playerUI(){
	cout << player->getName() << endl;
	cout << player->getCurrentHealth() << "/" << player->getMaxHealth() << endl;
	for (int x = 0; x < player->getCurrentCrystals(); x++) {
		cout << char(178) << " ";
	}
	for (int x = 0; x < player->getMaxCrystals() - player->getCurrentCrystals(); x++) {
		cout << char(176) << " ";
	}
	cout << endl;
	if (player->getNumberOfBuffs() > 0) {
		cout << "Buffs:" << endl;
		for (int i = 0; i < player->getNumberOfBuffs(); i++) {
			Effect buff = player->getBuffAt(i);
			cout << buff.name << " - " << buff.message << endl;
		}
	}

	if (player->getNumberOfDebuffs() > 0) {
		cout << "\nDebuffs:" << endl;
		for (int i = 0; i < player->getNumberOfDebuffs(); i++) {
			Effect buff = player->getDebuffAt(i);
			cout << buff.name << " - " << buff.message << endl;
		}
	}
	cout << endl;
}

void Battle::enemyUI() {
	for (int i = 0; i < numEnemies; i++) {
		cout << enemies[i]->getName() << "\t\t";
	}
	cout << endl;
	for (int i = 0; i < numEnemies; i++) {
		cout << enemies[i]->getCurrentHealth() << "/" << enemies[i]->getMaxHealth() << "\t\t";
	}
	cout << endl;
	for (int i = 0; i < numEnemies; i++) {
		for (int x = 0; x < enemies[i]->getCurrentCrystals(); x++) {
			cout << char(178) << " ";
		}
		for (int x = 0; x < enemies[i]->getMaxCrystals() - enemies[i]->getCurrentCrystals(); x++) {
			cout << char(176) << " ";
		}
		cout << "\t";
	}
}

bool Battle::healthCheck() { //makes sure there's actually something to fight
	if (player->getCurrentHealth() == 0) {
		return false;
	}
	for (int i = 0; i < numEnemies; i++) {
		if (enemies[i]->getCurrentHealth() > 0)
			return true;
	}
	return false;
}

void Battle::executePlayerAction(int playerChoice, int playerAttackChoice, int playerSkillChoice, Skill playerSkill) {
	switch (playerChoice) {
	case 1:
		damage(player, enemies[playerAttackChoice], player->attack());
		player->regenCrystals();
		break;
	case 2:
		cout << player->getName() << playerSkill.message << endl;
		if (!playerSkill.isAOE) {
			if (playerSkill.isBuff) {
				player->addBuff(playerSkill.effect);
			}
			else if (playerSkill.isDebuff) {
				enemies[playerAttackChoice]->addDebuff(playerSkill.effect);
			}
			else {
				damage(player, enemies[playerAttackChoice], playerSkill.damage);
			}
		}
		else {
			if (playerSkill.isDebuff) {
				for (int i = 0; i < numEnemies; i++)
					enemies[i]->addDebuff(playerSkill.effect);
			}
			else {
				for (int i = 0; i < numEnemies; i++)
					damage(player, enemies[i], playerSkill.damage);
			}
		}
		player->decreaseCurrentCrystals(playerSkill.cost);
		break;
	case 3:
		player->defend();
		player->regenCrystals();
		cout << endl;
		Sleep(1000);
		break;
	case 4:
		player->charge();
		cout << endl;
		Sleep(1000);
		break;
	}
}

void Battle::executeEnemyActions(int enemyChoices[]) {
	Skill enemySkill;
	for (int i = 0; i < numEnemies; i++) {
		if (enemies[i]->getCurrentHealth() == 0) continue; //makes sure a dead enemy does not attack;
		switch (enemyChoices[i]) {
		case 1:
			damage(enemies[i], player, enemies[i]->attack());
			Sleep(1000);
			enemies[i]->regenCrystals();
			break;
		case 2:
			enemySkill = enemies[i]->whichSkill();
			cout << enemies[i]->getName() << enemySkill.message << endl;
			if (enemySkill.isBuff) {
				if (enemySkill.isAOE) {
					for (int x = 0; x < numEnemies; x++)
						enemies[x]->addBuff(enemySkill.effect);
				}
				else {
					enemies[i]->addBuff(enemySkill.effect);
				}
			}
			else if (enemySkill.isDebuff) {
				player->addDebuff(enemySkill.effect);
			}
			else {
				damage(enemies[i], player, enemySkill.damage);
			}
			enemies[i]->decreaseCurrentCrystals(enemySkill.cost);
			cout << endl;
			break;
		case 4:
			enemies[i]->charge();
			cout << endl;
			Sleep(1000);
			break;
		}
	}
}

void Battle::durationTick() {
	player->decreaseAllEffectDurations();
	for (int i = 0; i < numEnemies; i++)
		enemies[i]->decreaseAllEffectDurations();
}

int Battle::getPlayerAttackChoice() {
	int playerAttackChoice;
	cout << "Which enemy do you want to attack? Enter 0 to return." << endl;
	for (int i = 0; i < numEnemies; i++)
		cout << i + 1 << ". " << enemies[i]->getName() << endl;
	cout << endl << "Enemy: ";
	cin >> playerAttackChoice;
	cin.ignore();
	if (playerAttackChoice == 0) return 0;
	while (!isValidAttack(playerAttackChoice)) {
		if (playerAttackChoice < 0 || playerAttackChoice > numEnemies) 
			cout << "Your choice is not an enemy. Please enter a valid choice." << endl;
		else
			cout << "That enemy is already dead. Please choose an enemy that is alive." << endl;
		cout << "Enemy: ";
		cin >> playerAttackChoice;
	}
	return playerAttackChoice;
}

bool Battle::isValidAttack(int playerAttackChoice) {
	return (playerAttackChoice > 0 && playerAttackChoice < numEnemies) || enemies[playerAttackChoice - 1]->getCurrentHealth() > 0;
}
