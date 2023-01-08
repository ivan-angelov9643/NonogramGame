#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include "Header.h"

using namespace std;

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

bool compareHashes(char* hash1, char* hash2) {
	for (int i = 0; i < HASH_SIZE; i++)
	{
		if (hash1[i] != hash2[i]) return false;
	}
	return true;
}

int stringSize(char* str) {
	int i = 0;
	while (str[i] != '\0') i++;
	return i;
}