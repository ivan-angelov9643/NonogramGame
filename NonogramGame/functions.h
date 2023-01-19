extern void readPictureFile(int**, int**, char**, int, char*);
extern int userLevel(char*);
extern void loadProfiles(char***);
extern bool registerProfile(char***, char**);
extern bool loginProfile(char***, char**);
extern bool usernameExists(char***, char*, int, char**);
extern bool isValid(char*, const char*);
extern char* hashPass(char*);
extern bool compareHashes(char*, char*);
extern int stringSize(char* str);
extern void printPicture(char**, int**, int**, int);
extern char* notLoggedIn(bool&, bool&);
extern void loggedIn(bool&, char*);
extern char** newGame(char*, int, char*, bool&, bool&, int&);
extern void fillInGamePicture(char**, int);
extern char* choosePictureFile(int);
extern int maxSubarraySize(int**, int);
extern void play(char*, char**, char**, int**, int**, int, bool&, bool&, int&);
extern bool validateInput(char, int, int, int);
extern void markRowAndColIfFilled(int, int, char**, char**, int, bool&, bool&);
extern void levelUp(char*);
extern void saveLastGame(char*, char*, char**, int);
extern void deleteLastGameSave(char*);
extern char** continueGame(char*, char*&, bool&, bool&, int&);
extern bool allRowsAreMarked(bool*, int);
extern bool readContinueGameFile(char*, int&, char*&, int&, char**);


