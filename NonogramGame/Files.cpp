#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Header.h"

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
	}
	userLvlFile.close();
	return userLvl;
}
