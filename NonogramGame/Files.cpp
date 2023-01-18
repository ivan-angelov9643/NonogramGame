#include <iostream>
#include <fstream>
#include <string>
#include "globals.h"
#include "functions.h"

using namespace std;

void readPictureFile(int** rows, int** cols, char** picture, int pictureSize, char* fileName) {
	fstream pictureFile;
	pictureFile.open(fileName, fstream::in);

	if (!pictureFile.is_open())
	{
		cout << "Error opening file" << endl;
		return;
	}

	for (int i = 0; i < pictureSize; i++)
	{
		pictureFile >> picture[i];
	}

	int i = 0;
	int j = 0;
	int numbersPerLineCount = 0;
	int linesCount = 0;
	int currentNumber = 0;
	int** currentArray = rows;

	while (linesCount <= pictureSize * 2) {
		if (linesCount == pictureSize + 1)
		{
			i = 0;
			j = 0;
			currentArray = cols;
		}
		if (numbersPerLineCount == 0)
		{
			pictureFile >> numbersPerLineCount;
			i = linesCount++ % pictureSize;
			j = 0;
		}
		else
		{
			pictureFile >> currentNumber;
			currentArray[i][j] = currentNumber;
			numbersPerLineCount--;
			j++;
		}
	}
	pictureFile.close();
}

int userLevel(char* user) {
	int userLvl = 1;
	fstream userLvlFile;
	userLvlFile.open(string(user) + "Lvl.txt", fstream::in | fstream::out);
	if (!userLvlFile) {
		userLvlFile.open(string(user) + "Lvl.txt", fstream::out);
		if (!userLvlFile) {
			cout << "Could not create file." << endl;
			return 0;
		}
		userLvlFile << userLvl << endl;
	}
	else {
		char* buffer = new char[LINE_SIZE] {0};
		userLvlFile.getline(buffer, LINE_SIZE);
		userLvl = buffer[0] - '0';
		delete[] buffer;
	}
	userLvlFile.close();
	return userLvl;
}

void levelUp(char* user) {
	int userLvl;
	fstream userLvlFile;
	userLvlFile.open(string(user) + "Lvl.txt", fstream::in);
	if (!userLvlFile.is_open())
	{
		cout << "Error opening file" << endl;
		return;
	}
	char* buffer = new char[LINE_SIZE] {0};
	userLvlFile.getline(buffer, LINE_SIZE);
	userLvl = buffer[0] - '0';
	userLvlFile.close();
	userLvlFile.open(string(user) + "Lvl.txt", fstream::trunc | fstream::out);
	userLvlFile << userLvl + 1 << endl;
	userLvlFile.close();
	delete[] buffer;
}

void saveLastGame(char* user, char* gameFileName, char** inGamePicture, int lives) {
	int size = stringSize(inGamePicture[0]);

	fstream userLastGameFile;
	userLastGameFile.open(string(user) + "LastGame.txt", fstream::out);
	if (!userLastGameFile.is_open()) {
		cout << "Could not create file." << endl;
		return;
	}
	userLastGameFile << gameFileName << endl;
	userLastGameFile << size << endl;
	userLastGameFile << lives << endl;
	for (int i = 0; i < size; i++)
	{
		userLastGameFile << inGamePicture[i] << endl;
	}

	userLastGameFile.close();
	cout << "Game saved successfully" << endl;
}

void deleteLastGameSave(char* user) {
	std::fstream userLastGameFile(string(user) + "LastGame.txt", fstream::in);
	if (userLastGameFile) {
		userLastGameFile.close();
		remove((string(user) + "LastGame.txt").c_str());
	}
}

