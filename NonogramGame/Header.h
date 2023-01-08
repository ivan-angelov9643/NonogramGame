#include <map>

using namespace std;

extern const int LINE_SIZE;
extern const int INPUT_SIZE;
extern const int HASH_SIZE;
extern const int MAX_USERNAME_AND_PASSWORD_SIZE;
extern const int MIN_USERNAME_AND_PASSWORD_SIZE;
extern const char* PROFILES_FILE_NAME;
extern const int SIZE;
extern const int FILENAME_SIZE;
extern const int LIVES;
extern const map<int, char**> PICTURES_FILES_NAMES;
extern void readPictureFile(int**, int**, char**, int, char*);
extern int userLevel(char*);
extern map<char*, char*> loadProfiles(map<char*, char*>);
extern bool registerProfile(map<char*, char*>, char**);
extern bool loginProfile(map<char*, char*>, char**);
extern bool usernameExists(map<char*, char*>, char*, int, char**);
extern bool isValid(char*, const char*);
extern char* hashPass(char*);
extern bool compareHashes(char*, char*);
extern int stringSize(char* str);
extern void printPicture(char**, int**, int**, int);
extern char* notLoggedIn(bool&, bool&);
extern void loggedIn(bool&, char*);
extern char** newGame(char*, bool&, bool&);
extern void fillInGamePicture(char**, int);
extern char* choosePictureFile(int);
extern int maxSubarraySize(int**, int);
extern void Play(char**, char**, int**, int**, int, bool&, bool&);
extern bool ValidateInput(char, int, int, int);
extern void MarkRowAndColIfFilled(int, int, char**, char**, int, bool&, bool&);
