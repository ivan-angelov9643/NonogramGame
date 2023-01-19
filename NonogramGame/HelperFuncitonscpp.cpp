#include <iostream>
#include "globals.h"
#include "functions.h"

using namespace std;

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

void fillInGamePicture(char** picture, int size) {
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			picture[i][j] = '*';
		}
	}
}

bool validateInput(char action, int i, int j, int size) {
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

char* choosePictureFile(int lvl) {
	int ind = rand() % 2;
	char* fileName = new char[FILENAME_SIZE];
	for (int i = 0; i < FILENAME_SIZE; i++)
	{
		fileName[i] = PICTURES_FILES_NAMES[lvl - 1][ind][i];
	}
	return fileName;
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

void markRowAndColIfFilled(int i, int j, char** solvedPicture, char** inGamePicture, int size, bool& markRow, bool& markCol) {
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

bool allRowsAreMarked(bool* markedRows, int size) {
	for (int i = 0; i < size; i++)
	{
		if (!markedRows[i])
		{
			return false;
		}
	}
	return true;
}

