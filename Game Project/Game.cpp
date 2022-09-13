#include <iostream>
#include <fstream>
#include <string>
#include "Character.h"
#include "Berserker.h"
#include "Battle.h"
#include "Map.h"
#include <typeinfo>
using namespace std;

//i am very sorry for anyone who has to read through this

void createNewSave(ofstream& upgradeFile) {
	upgradeFile.open("UpgradeSave.txt");
	upgradeFile << 0 << endl << 0 << endl << 0 << endl << 0 << endl << 0 << endl << 0 << endl;
	upgradeFile.close();
}

void upgradeMenu(Upgrades& u,Character* player) { //I think I've given up at this point
	while (true) {
		for (int i = 0; i < 40; i++) cout << endl;
		cout << "Upgrades: " << endl;
		cout << "1. Health(+20) " << u.health << "/5" << " - " << (u.health+1) * 100  << " essence" << endl;
		cout << "2. Armor(+2) " << u.armor << "/5" << " - " << (u.armor + 1) * 100 << " essence" << endl;
		cout << "3. Attack(+5) " << u.attack << "/5" << " - " << (u.attack + 1) * 100 << " essence" << endl;
		cout << "4. Evade(+2) " << u.evade << "/5" << " - " << (u.evade + 1) * 100 << " essence" << endl;
		cout << "5. Critical Chance(+4) " << u.crit << "/5" << " - " << (u.crit + 1) * 100 << " essence" << endl;
		cout << "Essence: " << u.essence << endl;
		cout << "What would you like to upgrade? Enter 0 to return to main menu." << endl;
		int choice;
		cin >> choice;
		cin.ignore();
		if (choice == 0) return;
		if (choice < 0 || choice > 5) {
			cout << "Enter a valid choice" << endl;
			cin >> choice;
			cin.ignore();
		}
		switch (choice) {
		case 1:
			if (u.health == 5) cout << "Already max." << endl;
			else if ((u.health + 1) * 100 > u.essence) cout << "Not enough essence." << endl;
			else {
				u.essence -= (u.health + 1) * 100;
				u.health++;
				cout << "Health upgraded to level " << u.health << "!" << endl;
			}
			break;
		case 2:
			if (u.armor == 5) cout << "Already max." << endl;
			else if ((u.armor + 1) * 100 > u.essence) cout << "Not enough essence." << endl;
			else {
				u.essence -= (u.armor + 1) * 100;
				u.armor++;
				cout << "Armor upgraded to level " << u.armor << "!" << endl;
			}
			break;
		case 3:
			if (u.attack == 5) cout << "Already max." << endl;
			else if ((u.attack + 1) * 100 > u.essence) cout << "Not enough essence." << endl;
			else {
				u.essence -= (u.attack + 1) * 100;
				u.attack++;
				cout << "Attack upgraded to level " << u.attack << "!" << endl;
			}
			break;
		case 4:
			if (u.evade == 5) cout << "Already max." << endl;
			else if ((u.evade + 1) * 100 > u.essence) cout << "Not enough essence." << endl;
			else {
				u.essence -= (u.evade + 1) * 100;
				u.evade++;
				cout << "Evade upgraded to level " << u.evade << "!" << endl;
			}
			break;
		case 5:
			if (u.crit == 5) cout << "Already max." << endl;
			else if ((u.crit + 1) * 100 > u.essence) cout << "Not enough essence." << endl;
			else {
				u.essence -= (u.crit + 1) * 100;
				u.crit++;
				cout << "Critical Chance upgraded to level " << u.crit << "!" << endl;
			}
			break;
		}
		cout << "Press enter to continue...";
		cin.ignore();
	}
}

void save(ofstream& upgradeFile,Upgrades u) {
	upgradeFile.open("UpgradeSave.txt");
	upgradeFile << u.health << endl;
	upgradeFile << u.armor << endl;
	upgradeFile << u.attack << endl;
	upgradeFile << u.evade << endl;
	upgradeFile << u.crit << endl;
	upgradeFile << u.essence << endl;
}

int main() {
	srand(time(NULL));
	cout << "Welcome to RPG game!" << endl;
	ifstream tester("UpgradeSave.txt");
	ofstream oSaveFile;
	ofstream oUpgradeFile;
	Upgrades u;
	if (!tester.is_open()) {
		cout << "No save file found. Would you like to make a new one? (y/n)" << endl;
		string a;
		cin >> a;
		cin.ignore();
		if (a == "y") {		
			createNewSave(oUpgradeFile);
		}
		else {
			cout << "ok" << endl;
			return 1;
		}
	}

	Character* saveBerserker = new Berserker(ifstream("Berserker.txt"));
	ifstream upgradeFile("upgradeSave.txt");
	upgradeFile >> u.health;
	upgradeFile >> u.armor;
	upgradeFile >> u.attack;
	upgradeFile >> u.evade;
	upgradeFile >> u.crit;
	upgradeFile >> u.essence;
	saveBerserker->setEssence(u.essence);
	saveBerserker->applyUpgrades(u);

	
	while (true) {
		for (int i = 0; i < 40; i++) cout << endl;
		cout << "RPG GAME" << endl; 
		cout << "-----------" << endl;
		cout << "1. Start run" << endl;
		cout << "2. Upgrades" << endl;
		cout << "3. Display status" << endl;
		cout << "4. Save and quit" << endl;
		int choice;
		cin >> choice;
		cin.ignore();
		if (choice < 1 || choice > 4) {
			cout << "Enter a valid choice" << endl;
			cin >> choice;
			cin.ignore();
		}
		if (choice == 1) {
			saveBerserker->setCurrentHealth(saveBerserker->getMaxHealth());
			Map theMap(saveBerserker);
			Battle nextBattle;
			for (int i = 0; i < 6; i++) {
				if (saveBerserker->getCurrentHealth() == 0) {
					cout << "\nYou died! Better luck next time!" << endl;
					cout << "Press enter to continue..." << endl;
					cin.ignore();
					break;
 				}
				int reward = (rand() % 100 + 100) * (i + 1);
				switch (theMap.nextLocation()) {
				case 0:
					nextBattle = theMap.generateBattle();
					nextBattle.executeBattle();
					if (saveBerserker->getCurrentHealth() == 0)
						break;
					cout << "Congrats! You won!" << endl;
					cout << "You gained " << reward << " essence!" << endl;
					cout << "Press enter to continue...";
					cin.ignore();

					u.essence += reward;
					break;
				case 1:
					theMap.campsite();
					break;
				case 2:
					nextBattle = theMap.generateBossBattle();
					nextBattle.executeBattle();
					if (saveBerserker->getCurrentHealth() == 0)
						break;
					cout << "Congrats! You won!" << endl;
					cout << "You gained " << reward << " essence along with additonal 500 for beating a boss!" << endl;
					cout << "Press enter to continue...";
					cin.ignore();

					u.essence += reward + 500;
					break;
				}
			}
		}
		if (choice == 2) {
			upgradeMenu(u, saveBerserker);
			saveBerserker->applyUpgrades(u);
			saveBerserker->setEssence(u.essence);
		}
		if (choice == 3) {
			for (int i = 0; i < 40; i++) cout << endl;
			saveBerserker->displayStatus();
			cout << "Press enter to continue...";
			cin.ignore();
		}
		if(choice == 4){
			save(oUpgradeFile, u);
			break;
		}
	}
	
	
	
	delete saveBerserker;
	
	
}