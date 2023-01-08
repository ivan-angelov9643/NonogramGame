#include <map>
#include "Header.h"

using namespace std; 

const int LINE_SIZE = 256;
const int INPUT_SIZE = 101;
const int HASH_SIZE = 30;
const int MAX_USERNAME_AND_PASSWORD_SIZE = 25;
const int MIN_USERNAME_AND_PASSWORD_SIZE = 3;
const char* PROFILES_FILE_NAME = "profiles.txt";
const int SIZE = 25;
const int FILENAME_SIZE = 20;
const int LIVES = 3;
const map<int, char**> PICTURES_FILES_NAMES = {
   { 1, new char* [2] {
   new char[FILENAME_SIZE] { "5x5picture1.txt" },
   new char[FILENAME_SIZE] { "5x5picture2.txt" }
   }},
   { 2, new char* [2] {
   new char[FILENAME_SIZE] { "10x10picture1.txt" },
   new char[FILENAME_SIZE] { "10x10picture2.txt" }
   }},
   { 3, new char* [2] {
   new char[FILENAME_SIZE] { "15x15picture1.txt" },
   new char[FILENAME_SIZE] { "15x15picture2.txt" }
   }},
   { 4, new char* [2] {
   new char[FILENAME_SIZE] { "20x20picture1.txt" },
   new char[FILENAME_SIZE] { "20x20picture2.txt" }
   }},
   { 5, new char* [2] {
   new char[FILENAME_SIZE] { "25x25picture1.txt" },
   new char[FILENAME_SIZE] { "25x25picture2.txt" }
   }},
};