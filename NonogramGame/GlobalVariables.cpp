#include "globals.h"

const int LINE_SIZE = 256;
const int INPUT_SIZE = 101;
const int HASH_SIZE = 30;
const int MAX_USERNAME_AND_PASSWORD_SIZE = 25;
const int MIN_USERNAME_AND_PASSWORD_SIZE = 3;
const char* PROFILES_FILE_NAME = "profiles.txt";
const int MAX_PROFILES = 30;
const int SIZE = 25;
const int FILENAME_SIZE = 20;
const int LIVES = 3;
const int PROFILE_INFORMATION_PIECES = 2;
const int LEVELS = 3;
const char*** PICTURES_FILES_NAMES = new const char** [LEVELS] {
	new const char* [2] {
		new const char[FILENAME_SIZE] { "5x5picture1.txt" },
			new const char[FILENAME_SIZE] { "5x5picture2.txt" }
	},
		new const char* [2] {
			new const char[FILENAME_SIZE] { "10x10picture1.txt" },
				new const char[FILENAME_SIZE] { "10x10picture2.txt" }
		},
			new const char* [2] {
				new const char[FILENAME_SIZE] { "15x15picture1.txt" },
					new const char[FILENAME_SIZE] { "15x15picture2.txt" }
			}
};