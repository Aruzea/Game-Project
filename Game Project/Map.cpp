#include "Map.h"
#include <fstream>
using namespace std;

Map::Map(Character* p) : player(p), locationCounter(1) {
	
	enemies = new Enemy[3];
	enemies[0] = Enemy(ifstream("Wolf.txt"));
	enemies[1] = Enemy(ifstream("Armored_wolf.txt"));
	enemies[2] = Enemy(ifstream("wolf_boss.txt"));
	/*int size;
	file >> size;
	enemies = new Enemy[size];
	int health, armor, maxCrystals, crystalR, evadeC, critC, critM, attackValue;
	string name, attackM, defendM, chargeM, type;

	for (int i = 0; i < size; i++){
		file >> name;
		file >> health;
		file >> armor;
		file >> maxCrystals;
		file >> crystalR;
		file >> evadeC;
		file >> critC;
		file >> critM;
		file >> attackValue;
		file.ignore();
		getline(file, attackM);
		getline(file, defendM);
		getline(file, chargeM);
		file >> type;
		
		enemies[i] = Enemy(name, health, armor, maxCrystals, crystalR, evadeC, critC, critM, attackValue, attackM, defendM, chargeM, type);
	}*/
}

int Map::nextLocation() {
	int r = 0;
	if (locationCounter == 3)
		r = 1;
	if (locationCounter == 6)
		r = 2;
	locationCounter++;
	return r;
}

Battle Map::generateBattle() { //thought about actually making an algorithim, gave up in 2 seconds cause it's crunch time~
	int locationCounter = this->locationCounter - 1;
	if (locationCounter == 1) {
		Enemy** e = new Enemy * [1];
		e[0] = new Enemy(enemies[0]);
		return Battle(player, e, 1);
	}

	if (locationCounter == 2){
		int rng = rand() % 2;
		if (rng == 0) {
			Enemy** e = new Enemy * [1];
			e[0] = new Enemy(enemies[0]);
			buffRandomStat(e[0], 3);
			return Battle(player, e, 1);
		}
		else if (rng == 1) {
			Enemy** e = new Enemy * [2];
			for (int i = 0; i < 2; i++) {
				e[i] = new Enemy(enemies[0]);
				e[i]->setName(e[i]->getName() + " " + to_string(i+1));
				buffRandomStat(e[i], 2);
			}
			return Battle(player, e, 2);
		}
	}

	if (locationCounter == 4) {
		int rng = rand() % 3;
		if (rng == 0) {
			Enemy** e = new Enemy * [1];
			e[0] = new Enemy(enemies[1]);
			return Battle(player, e, 1);
		}
		else if (rng == 1) {
			Enemy** e = new Enemy * [3];
			for (int i = 0; i < 3; i++) {
				e[i] = new Enemy(enemies[0]);
				e[i]->setName(e[i]->getName() + " " + to_string(i+1));
				buffRandomStat(e[i], 2);
			}
			return Battle(player, e, 3);
		}
		else if (rng == 2) {
			Enemy** e = new Enemy * [2];
			for (int i = 0; i < 2; i++) {
				e[i] = new Enemy(enemies[0]);
				e[i]->setName(e[i]->getName() + " " + to_string(i+1));
				buffRandomStat(e[i], 5);
			}
			return Battle(player, e, 2);
		}
	}

	if (locationCounter == 5) {
		int rng = rand() % 100;
		if (rng == 69) { // haha funny number
			Enemy** e = new Enemy * [1];
			e[0] = new Enemy(enemies[0]);
			e[0]->setName("Cracked wolf");
			buffRandomStat(e[0], 69);
			return Battle(player, e, 1);
		}
		else rng = rand() % 3;
		if (rng == 0) { 
			Enemy** e = new Enemy * [3];
			e[0] = new Enemy(enemies[0]);
			e[0]->setName("Wolf 1");
			e[1] = new Enemy(enemies[1]);
			e[2] = new Enemy(enemies[0]);
			e[2]->setName("Wolf 2");
			return Battle(player, e, 3);
		}
		else if (rng == 1) {
			Enemy** e = new Enemy * [2];
			for (int i = 0; i < 2; i++) {
				e[i] = new Enemy(enemies[1]);
				buffRandomStat(e[i], 2);
				e[i]->setName(e[i]->getName() + " " + to_string(i+1));
			}
			return Battle(player, e, 2);
		}
		else if (rng == 2) {
			Enemy** e = new Enemy * [3];
			for (int i = 0; i < 3; i++) {
				e[i] = new Enemy(enemies[0]);
				e[i]->setName(e[i]->getName() + " " + to_string(i+1));
				buffRandomStat(e[i], 5);
			}
			return Battle(player, e, 3);
		}
	}
}




Battle Map::generateBossBattle() {
	Enemy** e = new Enemy * [1];
	e[0] = new Enemy(enemies[2]);
	return Battle(player, e, 1);
}

void Map::campsite() {
	for (int i = 0; i < 40; i++) cout << endl;
	cout << "You are able to rest at a campsite. Restore 25% health." << endl;
	player->increaseCurrentHealth(round(player->getCurrentHealth() * .25));
	cout << "Press enter to continue...";
	cin.ignore();
}


void Map::buffRandomStat(Enemy* e, int numTimes) { //this is the point where I realize I only need to put srand() once for it to work WOOOOOOOOOOOOOOOOOOOOOOO
	for (int i = 0; i < numTimes; i++) {
		int rng = rand() % 5;
		switch (rng) {
		case 0:
			e->setMaxHealth(e->getMaxHealth() + 5);
			e->increaseCurrentHealth(10);
			break;
		case 1:
			e->setArmor(e->getArmor() + 2);
			break;
		case 2:
			e->setEvadeChance(e->getEvadeChance() + 2);
			break;
		case 3:
			e->setCritChance(e->getCritChance() + 2);
			break;
		case 4:
			e->setAttackValue(e->getAttackValue() + 3);
			break;
		}
	}
}