#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void ReadPictureFile(int**, int**, char**, int, const char*);
const int SIZE = 5;

int main()
{
	const char* fileName = "5x5picture1.txt";
	int** rows = new int* [SIZE];
	int** cols = new int* [SIZE];
	char** picture = new char* [SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		rows[i] = new int[SIZE] {0};
		cols[i] = new int[SIZE] {0};
		picture[i] = new char[SIZE + 1] {0};
	}
	ReadPictureFile(rows, cols, picture, 5, fileName);
	cout << "Rows:" << endl;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			cout << rows[i][j] << " ";
		}
		cout << endl;
	}
	cout << "Cols:" << endl;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			cout << cols[i][j] << " ";
		}
		cout << endl;
	}
	cout << "Picture:" << endl;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			cout << picture[i][j];
		}
		cout << endl;
	}

}

void ReadPictureFile(int** rows, int** cols, char** picture, int pictureSize, const char* fileName) {
	fstream pictureFile;
	pictureFile.open(fileName, fstream::in);

	if (!pictureFile.is_open())
	{
		cout << "Error opening file";
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
			//cout << "arr[" << i << "][" << j << "] = " << currentNumber << endl;
			numbersPerLineCount--;
			j++;
		}

	}

}




