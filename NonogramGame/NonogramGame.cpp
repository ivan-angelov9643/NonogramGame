#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Header.h"
#include <set>

using namespace std;

//delete arrays !!!!
int main()
{
	bool isLoggedIn = false;
	bool exit = false;
	char* user = nullptr;
	while (true)
	{
		if (exit)
		{
			delete[] user;
			return 0;
		}
		if (!isLoggedIn)
		{
			user = notLoggedIn(isLoggedIn, exit);
		}
		else
		{
			loggedIn(isLoggedIn, user);
		}
	}
}

void loggedIn(bool& loggedIn, char* user) {
	char* choice = new char[INPUT_SIZE];
	cout << "Welcome " << user << "!" << endl;
	char** inGamePicture = nullptr;
	bool gameFinished;
	bool won;
	int lives;
	char* fileName;
	int userLvl = userLevel(user);
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
				if (won) levelUp(user);
				inGamePicture = nullptr;
			}
			break;
		case '2':
			// continue game
			break;
		case '3':
			if (inGamePicture == nullptr) cout << "There's no current game to save" << endl;
			else saveLastGame(user, inGamePicture, lives);
			break;
		default:
			cout << "No such choice" << endl;
			break;
		}
	}
	delete[] choice;
}

void saveLastGame(char* user, char** inGamePicture, int lives) {
	int size = stringSize(inGamePicture[0]);

	fstream userLastGameFile;
	userLastGameFile.open(string(user) + "LastGame.txt", fstream::out);
	if (!userLastGameFile.is_open()) {
		cout << "Could not create file." << endl;
		return;
	}
	userLastGameFile << size / 5 << endl;
	userLastGameFile << lives << endl;
	for (int i = 0; i < size; i++)
	{
		userLastGameFile << inGamePicture[i] << endl;
	}

	userLastGameFile.close();
	cout << "Game saved successfully" << endl;
}

char** newGame(char* user, int userLvl, char* fileName, bool& gameFinished, bool& won, int& lives)
{
	//cout << user << ", your level is " << userLvl << endl;

	int** rows = new int* [SIZE];
	int** cols = new int* [SIZE];
	char** solvedPicture = new char* [SIZE];
	char** inGamePicture = new char* [SIZE];
	int size = userLvl * 5;
	for (int i = 0; i < SIZE; i++)
	{
		rows[i] = new int[SIZE] {0};
		cols[i] = new int[SIZE] {0};
		solvedPicture[i] = new char[SIZE + 1] {0};
		inGamePicture[i] = new char[SIZE + 1] {0};
	}
	readPictureFile(rows, cols, solvedPicture, size, fileName);
	fillInGamePicture(inGamePicture, size);
	lives = LIVES;
	Play(user, solvedPicture, inGamePicture, rows, cols, size, gameFinished, won, lives);
	return inGamePicture;
}

void Play(char* user, char** solvedPicture, char** inGamePicture, int** rows, int** cols,
	int size, bool& gameFinished, bool& won, int& lives) {
	cout << "f i j - fill cell with coordinates (i, j)" << endl;
	cout << "e i j - empty cell with coordinates (i, j)" << endl;
	cout << "0 - back" << endl;
	int i, j;
	char action;
	set<int> markedRows = {};
	while (!gameFinished)
	{
		printPicture(inGamePicture, rows, cols, size);
		if (won) break;
		cin >> action;
		if (action == '0') return;
		cin >> i >> j;
		if (!ValidateInput(action, i, j, size)) continue;
		if (action == 'f' && solvedPicture[i][j] == '.' || action == 'e' && solvedPicture[i][j] == '#')
		{
			cout << "Wrong!" << endl;
			lives -= 1;
			cout << "Lives remaining: " << lives << endl;
			if (lives == 0) {
				gameFinished = true;
				break;
			}
		}
		inGamePicture[i][j] = solvedPicture[i][j];
		bool markRow = true;
		bool markCol = true;
		MarkRowAndColIfFilled(i, j, solvedPicture, inGamePicture, size, markRow, markCol);
		if (markRow) markedRows.insert(i);
		if (markedRows.size() == size) won = true;
	}
	if (!won) cout << "Game Over!" << endl;
	if (won) cout << "You won!" << endl;
}

void deleteLastGameSave(char* user) {
	std::fstream userLastGameFile(string(user) + "LastGame.txt", fstream::in);
	if (userLastGameFile) {
		userLastGameFile.close();
		remove((string(user) + "LastGame.txt").c_str());
	}
}

void MarkRowAndColIfFilled(int i, int j, char** solvedPicture, char** inGamePicture, int size, bool& markRow, bool& markCol) {
	for (int k = 0; k < size; k++)
	{
		if (solvedPicture[i][k] == '#' && inGamePicture[i][k] != '#') markRow = false;
		if (solvedPicture[k][j] == '#' && inGamePicture[k][j] != '#') markCol = false;
	}
	if (markRow || markCol)
	{
		for (int k = 0; k < size; k++)
		{
			if (markRow) inGamePicture[i][k] = solvedPicture[i][k];
			if (markCol) inGamePicture[k][j] = solvedPicture[k][j];
		}
	}
}

void printPicture(char** picture, int** rows, int** cols, int size) {
	int maxSizeRow = maxSubarraySize(rows, size);
	int maxSizeCol = maxSubarraySize(cols, size);
	for (int i = maxSizeCol - 1; i >= 0; i--)
	{
		for (int i = 0; i < maxSizeRow + 3; i++) cout << ' ';
		for (int j = 0; j < size; j++)
		{
			if (cols[j][i] == 0) cout << ' ';
			else cout << cols[j][i];
		}
		cout << endl;
	}
	cout << endl;
	for (int i = 0; i < maxSizeRow + 3; i++) cout << ' ';
	for (int i = 0; i < size; i++) cout << i;
	cout << endl;
	for (int i = 0; i < size; i++)
	{
		for (int j = maxSizeRow - 1; j >= 0; j--)
		{
			if (rows[i][j] == 0) cout << ' ';
			else cout << rows[i][j];
		}
		cout << ' ' << i << ' ';
		for (int j = 0; j < size; j++)
		{
			cout << picture[i][j];
		}
		cout << endl;
	}
}

char* notLoggedIn(bool& loggedIn, bool& exit) {
	cout << "Welcome to Nonogram!" << endl;
	map<char*, char*> profiles;
	profiles = loadProfiles(profiles);
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
	delete[] choice;
	return user;
}

bool ValidateInput(char action, int i, int j, int size) {
	if (action != 'f' && action != 'e')
	{
		cout << "Action must be \'a\' or \'e\'";
	}
	if (i < 0 || i >= size || j < 0 || j >= size)
	{
		cout << "Coordinates must be >= 0 and < " << size << endl;
		return false;
	}
	return true;
}

void fillInGamePicture(char** picture, int size) {
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			picture[i][j] = '*';
		}
	}
}

char* choosePictureFile(int lvl) {
	int ind = rand() % 2;
	return PICTURES_FILES_NAMES.at(1)[0];
	//return PICTURES_FILES_NAMES.at(lvl)[ind];
}

int maxSubarraySize(int** arr, int size) {
	int maxSize = 0;
	for (int i = 0; i < size; i++)
	{
		int currentSize = 0;
		for (int j = 0; j < size; j++)
		{
			if (arr[i][j] == 0) {
				currentSize = j;
				break;
			}
		}
		if (currentSize > maxSize) maxSize = currentSize;
	}
	return maxSize;
}

