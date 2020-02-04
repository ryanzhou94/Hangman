#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cctype>

#define NUMBER_OF_LIVES 8

struct player{
    int lives;                  // available wrong guess
    int correctGuessing;        // the number of correct guessing
    char * availableList;       // available guessing list of letters
    char * currentGuessing;     // current (correct) guessing
    const char * realWord;      // answer
};
typedef struct player Player;
typedef Player * PtrToPlayer;

void initializePlayer(PtrToPlayer ptrToPlayer);
int getARandom(int boundary);
void printPrompt(Player player);
void validateInput(PtrToPlayer ptrToPlayer);
void validateLetter(PtrToPlayer ptrToPlayer, char letter);
bool isLetter(char c);
void freePlayer(PtrToPlayer ptrToPlayer);
int checkAvailable(const char * str, char letter);
void endGame(Player player);
void flushBuffer();
bool ifContinue();

const char * wordList[] = {"blue", "red", "yellow", "black", "white",
                           "orange", "pink", "green", "grey", "purple",
                           "apple", "banana", "pear", "pineapple", "peach"};

int main() {
    // initialize the random number generator seed
    srand((unsigned int)time(NULL));
    do{
        // create a player
        Player player;

        // initialize the player
        initializePlayer(&player);

        // print prompt with player's information
        printPrompt(player);

        // validate input from the player
        validateInput(&player);

        // print prompt when the game is end
        endGame(player);

        // free the memory allocated to the player
        freePlayer(&player);

        // Another game? y: continue;   n: quit;    otherwise: keep asking
        printf("Try again? (y/n)");
    } while (ifContinue());

    return 0;
}


// Get a random number in the range of [0, boundary - 1]
int getARandom(int boundary){
    return rand() % (boundary);
}

// Initialize the Player's information
void initializePlayer(PtrToPlayer ptrToPlayer){
    ptrToPlayer->lives = NUMBER_OF_LIVES;
    ptrToPlayer->availableList = strdup("abcdefghijklmnopqrstuvwxyz");
    ptrToPlayer->correctGuessing = 0;
    ptrToPlayer->realWord = wordList[getARandom(sizeof(wordList)/ sizeof(wordList[0]))];
    ptrToPlayer->currentGuessing = (char *)malloc((strlen(ptrToPlayer->realWord) + 1) * sizeof(char));

    for (int i = 0; i < strlen(ptrToPlayer->realWord); ++i) {
        ptrToPlayer->currentGuessing[i] = '-';
    }
    ptrToPlayer->currentGuessing[strlen(ptrToPlayer->realWord)] = '\0';
}

// Print prompt/hints for the player
void printPrompt(Player player){
    printf("\nGuess a letter for the word: '%s'\n", player.currentGuessing);
    printf("You have %d wrong guesses left, and have guessed %d letters out of %lu so far.\n", player.lives, player.correctGuessing, strlen(player.realWord));
    printf("Available letters are: %s\n", player.availableList);
}

// Validate the input from the player
void validateInput(PtrToPlayer ptrToPlayer){
    while(ptrToPlayer->correctGuessing < strlen(ptrToPlayer->realWord)){
        int letter = getchar();
        while(letter != '\n'){
            letter = tolower(letter);
            validateLetter(ptrToPlayer, letter);
            if (ptrToPlayer->lives <= 0 || ptrToPlayer->correctGuessing == strlen(ptrToPlayer->realWord)){
				flushBuffer();
                return;
            }
            letter = getchar();
        }
    }
}

// Validate one letter: 1. whether it is a letter 2. whether it is have been guessed 3. whether it is correct
void validateLetter(PtrToPlayer ptrToPlayer, char letter){
    if (isLetter(letter)){
        if (checkAvailable(ptrToPlayer->availableList, letter) != -1){
            ptrToPlayer->availableList[checkAvailable(ptrToPlayer->availableList, letter)] = '.';
			int index = 0;
			bool correct = false;
			while (ptrToPlayer->realWord[index] != '\0') {
				if (ptrToPlayer->realWord[index] == letter) {
					correct = true;
					ptrToPlayer->correctGuessing++;
					ptrToPlayer->currentGuessing[index] = letter;
				}
				index++;
			}
			if (correct) {
				printf("\n*** Correct guess - '%c' IS in the word ***\n", letter);
			}
			else {
				ptrToPlayer->lives--;
				printf("\n!!! Wrong guess - '%c' is not in the word !!!\n", letter);
			}
        } else {
            printf("You have already guessed '%c'!\n", letter);
        }
		printPrompt(*ptrToPlayer);
    } else {
        printf("'%c' is not a valid letter! Try again.\n", letter);
    }
	
}

// Check if the character is an English letter
bool isLetter(char c){
    return ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

// Free the memory allocated to the player
void freePlayer(PtrToPlayer ptrToPlayer){
    free(ptrToPlayer->currentGuessing);
    free(ptrToPlayer->availableList);
}

// Check if a letter is in the available list, if yes then return its index in the list, otherwise return -1
int checkAvailable(const char * str, char letter){
     int index = 0;
     while(str[index] != '\0'){
         if (str[index] == letter){
             return index;
         }
         index++;
     }
     return -1;
}

// Print prompt for the end of the game
void endGame(Player player){
    if (player.correctGuessing == strlen(player.realWord)){
        printf("Well done! The word was '%s'. You finished with %d wrong guesses left.\n", player.realWord, player.lives);
    } else {
        printf("You failed! The word was '%s'\n", player.realWord);
    }
}

// Flush invalid input in the input buffer
void flushBuffer(){
    while (getchar() != '\n'){
        // do nothing
    }
}

// Receive response from the player if he/she wants to continue the game
bool ifContinue() {
    char response = getchar();
	while (response != 'y' && response != 'n') {
		response = getchar();
	}
    flushBuffer();
    return response == 'y';
}