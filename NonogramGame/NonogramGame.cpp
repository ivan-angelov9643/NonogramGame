/**
*
* Solution to course project # 1
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2022/2023
*
* @author Ivan Angelov
* @idnumber 9MI0600190
* @compiler VC
*
* cpp file with main logic
*
*/

#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"
#include "functions.h"

using namespace std;

int main()
{
	bool isLoggedIn = false;
	bool exit = false;
	char* user = nullptr;
	while (true)
	{
		if (exit)
		{
			for (int i = 0; i < LEVELS; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					delete[] PICTURES_FILES_NAMES[i][j];
				}
				delete[] PICTURES_FILES_NAMES[i];
			}
			delete[] PICTURES_FILES_NAMES;
			return 0;
		}
		if (!isLoggedIn)
		{
			user = notLoggedIn(isLoggedIn, exit);
		}
		else
		{
			loggedIn(isLoggedIn, user);
			delete[] user;
		}
	}
}

char* notLoggedIn(bool& loggedIn, bool& exit) {
	cout << "Welcome to Nonogram!" << endl;
	char*** profiles = new char** [MAX_PROFILES];
	for (int i = 0; i < MAX_PROFILES; i++)
	{
		profiles[i] = new char* [PROFILE_INFORMATION_PIECES] {nullptr};
	}
	loadProfiles(profiles);
	char* choice = new char[INPUT_SIZE];
	bool success = false;
	char* user = nullptr;
	while (!success) {
		if (exit) break;
		cout << "0 - Exit" << endl;
		cout << "1 - Register" << endl;
		cout << "2 - Login" << endl;
		cin >> choice;
		switch (choice[0])
		{
		case '0':
			cout << "Closing game" << endl;
			exit = true;
			break;
		case '1':
			success = registerProfile(profiles, &user);
			break;
		case '2':
			success = loginProfile(profiles, &user);
			break;
		default:
			cout << "No such choice" << endl;
			break;
		}
	}
	loggedIn = true;
	for (int i = 0; i < MAX_PROFILES; i++)
	{
		for (int j = 0; j < PROFILE_INFORMATION_PIECES; j++)
		{
			delete[] profiles[i][j];
		}
		delete[] profiles[i];
	}
	delete[] profiles;
	delete[] choice;
	return user;
}

void loggedIn(bool& loggedIn, char* user) {
	char* choice = new char[INPUT_SIZE];
	char** inGamePicture = nullptr;
	bool gameFinished;
	bool won;
	int lives;
	char* fileName = nullptr;
	int userLvl = userLevel(user);
	cout << "Welcome " << user << "!" << endl;
	cout << "You are lvl " << userLvl << endl;
	while (true)
	{
		gameFinished = false;
		won = false;
		if (!loggedIn) break;
		cout << "0 - Logout" << endl;
		cout << "1 - New game" << endl;
		cout << "2 - Continue last game" << endl;
		cout << "3 - Save current game" << endl;
		cin >> choice;
		switch (choice[0])
		{
		case '0':
			cout << "Logging out" << endl;
			loggedIn = false;
			break;
		case '1':
			deleteLastGameSave(user);
			fileName = choosePictureFile(userLvl);
			inGamePicture = newGame(user, userLvl, fileName, gameFinished, won, lives);
			if (gameFinished) {
				if (won && userLevel(user) != 3) {
					levelUp(user);
					userLvl += 1;
					cout << "Level up to lvl " << userLvl << endl;
				}
				delete[] fileName;
				for (int i = 0; i < SIZE; i++)
				{
					delete[] inGamePicture[i];
				}
				delete[] inGamePicture;
				inGamePicture = nullptr;
			}
			break;
		case '2':
			inGamePicture = continueGame(user, fileName, gameFinished, won, lives);
			if (gameFinished) {
				deleteLastGameSave(user);
				if (won && userLevel(user) != 3) {
					levelUp(user);
					userLvl += 1;
					cout << "Level up to lvl " << userLvl << endl;
				}
				delete[] fileName;
				for (int i = 0; i < SIZE; i++)
				{
					delete[] inGamePicture[i];
				}
				delete[] inGamePicture;
				inGamePicture = nullptr;
			}
			break;
		case '3':
			if (inGamePicture == nullptr) cout << "There's no current game to save" << endl;
			else {
				saveLastGame(user, fileName, inGamePicture, lives);
				delete[] fileName;
			}
			break;
		default:
			cout << "There is no such option" << endl;
			break;
		}
	}
	if (inGamePicture != nullptr)
	{
		for (int i = 0; i < SIZE; i++)
		{
			delete[] inGamePicture[i];
		}
		delete[] inGamePicture;
	}
	if (fileName != nullptr)
	{
		delete[] fileName;
	}
	delete[] choice;
}

char** newGame(char* user, int userLvl, char* fileName, bool& gameFinished, bool& won, int& lives)
{
	int** rows = new int* [SIZE];
	int** cols = new int* [SIZE];
	char** solvedPicture = new char* [SIZE];
	char** inGamePicture = new char* [SIZE];
	int size = userLvl * 5;
	for (int i = 0; i < SIZE; i++)
	{
		rows[i] = new int[SIZE] {0};
		cols[i] = new int[SIZE] {0};
		solvedPicture[i] = new char[SIZE] {0};
		inGamePicture[i] = new char[SIZE] {0};
	}
	readPictureFile(rows, cols, solvedPicture, size, fileName);
	fillInGamePicture(inGamePicture, size);
	lives = LIVES;
	play(user, solvedPicture, inGamePicture, rows, cols, size, gameFinished, won, lives);
	for (int i = 0; i < SIZE; i++)
	{
		delete[] rows[i];
		delete[] cols[i];
		delete[] solvedPicture[i];
	}
	delete[] rows;
	delete[] cols;
	delete[] solvedPicture;
	return inGamePicture;
}

char** continueGame(char* user, char*& fileName, bool& gameFinished, bool& won, int& lives) {
	int** rows = new int* [SIZE];
	int** cols = new int* [SIZE];
	char** solvedPicture = new char* [SIZE];
	char** inGamePicture = new char* [SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		rows[i] = new int[SIZE] {0};
		cols[i] = new int[SIZE] {0};
		solvedPicture[i] = new char[SIZE + 1] {0};
		inGamePicture[i] = new char[SIZE + 1] {0};
	}
	int size = 0;
	if (!readContinueGameFile(user, size, fileName, lives, inGamePicture))
	{
		return nullptr;
	}
	readPictureFile(rows, cols, solvedPicture, size, fileName);
	play(user, solvedPicture, inGamePicture, rows, cols, size, gameFinished, won, lives);
	for (int i = 0; i < SIZE; i++)
	{
		delete[] rows[i];
		delete[] cols[i];
		delete[] solvedPicture[i];
	}
	delete[] rows;
	delete[] cols;
	delete[] solvedPicture;
	return inGamePicture;
}

void play(char* user, char** solvedPicture, char** inGamePicture, int** rows, int** cols,
	int size, bool& gameFinished, bool& won, int& lives) {
	cout << "f i j - fill cell with coordinates (i, j)" << endl;
	cout << "e i j - empty cell with coordinates (i, j)" << endl;
	cout << "0 - back" << endl;
	cout << "Lives remaining: " << lives << endl;
	int i, j;
	char action;
	bool* markedRows = new bool[SIZE] {0};
	while (!gameFinished)
	{
		printPicture(inGamePicture, rows, cols, size);
		if (won) {
			gameFinished = true;
			break;
		}
		cin >> action;
		if (action == '0') return;
		cin >> i >> j;
		if (cin.fail()) {
			cout << "Coordinates must be integers" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (!validateInput(action, i, j, size)) {
			continue;
		}
		if (action == 'f' && solvedPicture[i][j] == '.' || action == 'e' && solvedPicture[i][j] == '#')
		{
			cout << "Wrong!" << endl;
			lives -= 1;
			cout << "Lives remaining: " << lives << endl;
		}
		inGamePicture[i][j] = solvedPicture[i][j];
		bool markRow = true;
		bool markCol = true;
		markRowAndColIfFilled(i, j, solvedPicture, inGamePicture, size, markRow, markCol);
		if (markRow) markedRows[i] = true;
		if (allRowsAreMarked(markedRows, size) && lives != 0) won = true;
		if (lives == 0) {
			gameFinished = true;
		}
	}
	if (!won) cout << "Game Over!" << endl;
	if (won) cout << "You won!" << endl;
	delete[] markedRows;
}
