#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

void readPictureFile(int**, int**, char**, int, const char*);
void printPicture(char**, int);
bool registerProfile(map<char*, char*>, char**);
bool loginProfile(map<char*, char*>, char**);
bool usernameExists(map<char*, char*>, char*, int, char**);
int stringSize(char* str);
map<char*, char*> loadProfiles(map<char*, char*>);
bool compareHashes(char*, char*);
bool isValid(char*, const char*);
char* hashPass(char*);
char* notLoggedIn(bool&, bool&);
void loggedIn(bool&, char*);

const int SIZE = 5;
const int INPUT_SIZE = 101;
const int LINE_SIZE = 256;
const int MAX_USERNAME_AND_PASSWORD_SIZE = 25;
const int MIN_USERNAME_AND_PASSWORD_SIZE = 3;
const int HASH_SIZE = 30;
const char* PROFILES_FILE_NAME = "profiles.txt";

//delete arrays !!!!
int main()
{
	bool isLoggedIn = false;
	bool exit = false;
	//bool logout = false;
	char* currentUser = nullptr;
	while (true)
	{
		if (exit)
		{
			delete[] currentUser;
			return 0;
		}
		if (!isLoggedIn)
		{
			currentUser = notLoggedIn(isLoggedIn, exit);
		}
		else 
		{
			loggedIn(isLoggedIn, currentUser);
		}
	}
	
	
}

void loggedIn(bool& loggedIn, char* currentUser) {
	char* choice = new char[INPUT_SIZE];
	cout << "Welcome " << currentUser << "!" << endl;
	while (true)
	{
		if (!loggedIn) break;
		cout << "0 - Logout" << endl;
		cout << "1 - New game" << endl;
		cout << "2 - Continue game" << endl;
		cout << "3 - Save progress" << endl;
		cin >> choice;
		switch (choice[0])
		{
		case '0':
			cout << "Logging out" << endl;
			loggedIn = false;
			break;
		case '1':
			//new game
			break;
		case '2':
			// continue game 
			break;
		case '3':
			// continue game 
			break;
		default:
			cout << "No such choice" << endl;
			break;
		}
	}
	delete[] choice;
}

char* notLoggedIn(bool& loggedIn, bool& exit) {
	cout << "Welcome to Nonogram!" << endl;
	map<char*, char*> profiles;
	profiles = loadProfiles(profiles);
	char* choice = new char[INPUT_SIZE];
	bool success = false;
	char* currentUser = nullptr;
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
			success = registerProfile(profiles, &currentUser);
			break;
		case '2':
			success = loginProfile(profiles, &currentUser);
			break;
		default:
			cout << "No such choice" << endl;
			break;
		}
	}
	loggedIn = true;
	delete[] choice;
	return currentUser;
}

bool registerProfile(map<char*, char*> profiles, char** currentUser) {
	while (true) {
		char* username = new char[INPUT_SIZE];
		char* password = new char[INPUT_SIZE];

		cout << "---Register---" << endl;
		cout << "0 - back" << endl;

		cout << "Enter username: ";
		cin >> username;
		if (username[0] == '0' && username[1] == 0) {
			delete[] username;
			delete[] password;
			return false;
		}
		if (!isValid(username, "Username")) continue;
		if (usernameExists(profiles, username, stringSize(username), nullptr))
		{
			cout << "Username already exists" << endl;
			continue;
		}

		cout << "Enter password: ";
		cin >> password;
		if (!isValid(password, "Password")) continue;


		fstream profilesFile;
		profilesFile.open(PROFILES_FILE_NAME, fstream::out | fstream::app);

		if (!profilesFile.is_open())
		{
			cout << "Error opening file" << endl;
			return false;
		}

		profilesFile << username << ":" << hashPass(password) << endl;
		profilesFile.close();

		*currentUser = username;
		delete[] password;
		return true;
	}
}

bool loginProfile(map<char*, char*> profiles, char** currentUser) {
	while (true) {
		char* username = new char[INPUT_SIZE] {0};
		char* password = new char[INPUT_SIZE] {0};
		char* usernamesPasswordHash;

		cout << "---Login---" << endl;
		cout << "0 - back" << endl;
		cout << "Enter username: ";
		cin >> username;
		if (username[0] == '0' && username[1] == 0) 
		{
			delete[] username;
			delete[] password;
			return false;
		}
		if (!usernameExists(profiles, username, stringSize(username), &usernamesPasswordHash))
		{
			cout << "Username doesn't exist" << endl;
			continue;
		}

		cout << "Enter password: ";
		cin >> password;

		char* currentPasswordHash = hashPass(password);
		if (!compareHashes(currentPasswordHash, usernamesPasswordHash))
		{
			cout << "Wrong password" << endl;
			continue;
		}
		*currentUser = username;
		delete[] password;
		return true;
	}
}

bool usernameExists(map<char*, char*> profiles, char* username, int size, char** passwordHash) {
	map<char*, char*>::iterator it;

	bool exists;
	for (it = profiles.begin(); it != profiles.end(); it++)
	{
		char* currentUsername = it->first;
		int currentUsernameSize = stringSize(currentUsername);
		int biggerSize = size > currentUsernameSize ? size : currentUsernameSize;
		exists = true;
		for (int i = 0; i < biggerSize; i++)
		{
			if (currentUsername[i] != username[i]) {
				exists = false;
				break;
			}
		}
		if (exists)
		{
			if (passwordHash != nullptr) *passwordHash = it->second;
			return true;
		}
	}
	return false;
}

bool isValid(char* str, const char* title) {
	int size = stringSize(str);
	if (size > MAX_USERNAME_AND_PASSWORD_SIZE)
	{
		cout << title << " size must be maximum " << MAX_USERNAME_AND_PASSWORD_SIZE << " characters" << endl;
		return false;
	}
	if (size < MIN_USERNAME_AND_PASSWORD_SIZE)
	{
		cout << title << " size must be minimum " << MIN_USERNAME_AND_PASSWORD_SIZE << " characters" << endl;
		return false;
	}
	for (int i = 0; i < size; i++)
	{
		if (!(str[i] >= '0' && str[i] <= '9' ||
			str[i] >= 'A' && str[i] <= 'Z' ||
			str[i] >= 'a' && str[i] <= 'z' ||
			str[i] == '.' || str[i] == '_'))
		{
			cout << title << " can contain only letters, digits, dots (.) and underscores (_)" << endl;
			return false;
		}
	}
	return true;
}

map<char*, char*> loadProfiles(map<char*, char*> profiles) {
	fstream profilesFile;
	profilesFile.open(PROFILES_FILE_NAME, std::fstream::in);

	if (!profilesFile.is_open()) {
		cout << "Error opening file" << endl;
		return profiles;
	}

	char* buffer = new char[LINE_SIZE] {0};
	int i, j;
	while (profilesFile.getline(buffer, LINE_SIZE)) {
		i = 0;
		j = 0;
		char* username = new char[INPUT_SIZE] {0};
		char* passwordHash = new char[INPUT_SIZE] {0};
		while (buffer[i] != 0 && buffer[i] != ':') {
			username[j++] = buffer[i++];
		}
		j = 0;
		i++;
		while (buffer[i] != 0) {
			passwordHash[j++] = buffer[i++];
		}
		profiles[username] = passwordHash;
	}
	delete[] buffer;
	return profiles;
}

int stringSize(char* str) {
	int i = 0;
	while (str[i] != '\0') i++;
	return i;
}

void Test() {
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
	readPictureFile(rows, cols, picture, 5, fileName);
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
	printPicture(picture, SIZE);
}

void printPicture(char** picture, int size) {
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cout << picture[i][j];
		}
		cout << endl;
	}
}

//http://www.cse.yorku.ca/~oz/hash.html
char* hashPass(char* s) {
	size_t h = 5381;
	int c;
	while (c = *s++) h = ((h << 5) + h) + c;
	string temp = to_string(h);
	char* hash = new char[HASH_SIZE] {0};
	for (int i = 0; i < temp.size(); i++) hash[i] = temp[i];
	return hash;
}

void readPictureFile(int** rows, int** cols, char** picture, int pictureSize, const char* fileName) {
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
			//cout << "arr[" << i << "][" << j << "] = " << currentNumber << endl;
			numbersPerLineCount--;
			j++;
		}
	}
	pictureFile.close();
}


bool compareHashes(char* hash1, char* hash2) {
	for (int i = 0; i < HASH_SIZE; i++)
	{
		if (hash1[i] != hash2[i]) return false;
	}
	return true;
}


