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
			delete[] user;
			for (int i = 0; i < LEVELS; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					delete[] PICTURES_FILES_NAMES[i][j];
				}
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
		}
	}
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
				if (won && userLevel(user) != 3) levelUp(user);
				delete[] fileName;
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
					cout << "Level up to lvl 2" << endl;
				}
				delete[] fileName;
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
			cout << "No such choice" << endl;
			break;
		}
	}
	delete[] choice;
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
	fstream userLastGameFile;
	userLastGameFile.open(string(user) + "LastGame.txt", fstream::in);
	if (!userLastGameFile.is_open())
	{
		cout << "There is no last game save" << endl;
		return nullptr;
	}
	char* buffer = new char[LINE_SIZE] {0};
	userLastGameFile.getline(buffer, LINE_SIZE);
	fileName = new char[LINE_SIZE] {0};
	for (int i = 0; i < LINE_SIZE; i++)
	{
		if (buffer[i] == 0) break;
		fileName[i] = buffer[i];
	}
	userLastGameFile.getline(buffer, LINE_SIZE);
	int size = 0;
	if (LINE_SIZE >= 2)
	{
		size = stringSize(buffer) == 1 ? buffer[0] - '0' : ((buffer[0] - '0') * 10 + (buffer[1] - '0'));
	}
	userLastGameFile.getline(buffer, LINE_SIZE);
	lives = buffer[0] - '0';
	for (int i = 0; i < size; i++)
	{
		userLastGameFile.getline(buffer, LINE_SIZE);
		for (int j = 0; j < size; j++)
		{
			inGamePicture[i][j] = buffer[j];
		}
	}
	readPictureFile(rows, cols, solvedPicture, size, fileName);
	Play(user, solvedPicture, inGamePicture, rows, cols, size, gameFinished, won, lives);
	delete[] buffer;
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
		solvedPicture[i] = new char[SIZE + 1] {0};
		inGamePicture[i] = new char[SIZE + 1] {0};
	}
	readPictureFile(rows, cols, solvedPicture, size, fileName);
	fillInGamePicture(inGamePicture, size);
	lives = LIVES;
	Play(user, solvedPicture, inGamePicture, rows, cols, size, gameFinished, won, lives);
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

void Play(char* user, char** solvedPicture, char** inGamePicture, int** rows, int** cols,
	int size, bool& gameFinished, bool& won, int& lives) {
	cout << "f i j - fill cell with coordinates (i, j)" << endl;
	cout << "e i j - empty cell with coordinates (i, j)" << endl;
	cout << "0 - back" << endl;
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
		if (markRow) markedRows[i] = true;
		if (AllRowsAreMarked(markedRows, size)) won = true;
	}
	if (!won) cout << "Game Over!" << endl;
	if (won) cout << "You won!" << endl;
}

bool AllRowsAreMarked(bool* markedRows, int size) {
	for (int i = 0; i < size; i++)
	{
		if (!markedRows[i])
		{
			return false;
		}
	}
	return true;
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
		for (int i = 0; i < 2 * maxSizeRow + 1; i++) cout << ' ';
		for (int j = 0; j < size; j++)
		{
			if (cols[j][i] == 0) {
				cout << "  ";
			}
			else if (cols[j][i] >= 10)
			{
				cout << cols[j][i];
			}
			else {
				cout << cols[j][i] << ' ';
			}
		}
		cout << endl;
	}
	cout << endl;
	for (int i = 0; i < size; i++)
	{
		for (int j = maxSizeRow - 1; j >= 0; j--)
		{
			if (rows[i][j] == 0) {
				cout << "  ";
			}
			else if (rows[i][j] >= 10)
			{
				cout << rows[i][j];
			}
			else {
				cout << rows[i][j] << ' ';
			}
		}
		cout << ' ';
		for (int j = 0; j < size; j++)
		{
			cout << picture[i][j] << ' ';
		}
		cout << endl;
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
			delete[] profiles[i][j]; // fix it
		}
		delete[] profiles[i];
	}
	delete[] profiles;
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
	char* fileName = new char[FILENAME_SIZE];
	for (int i = 0; i < FILENAME_SIZE; i++)
	{
		fileName[i] = PICTURES_FILES_NAMES[lvl - 1][ind][i];
	}
	return fileName;
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

