#define _CRT_SECURE_NO_WARNINGS
#include "utilities.h"
#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;

const char WORDFILENAME[] = "C:/Users/KV COMP1/Desktop/CS31/Project5/smallwords.txt"; // file with all possible words
const int maxWords = 9000; // initialize max amount of words
const int nWords = 7265; // number of words in dictionary
const int MAXWORDLENGTH = 6;

char w[maxWords][MAXWORDLENGTH + 1]; // initialize 

// arrays to help determine silvers versus golds
char help[MAXWORDLENGTH + 1] = { 'N', 'N', 'N', 'N', 'N', 'N', '\0' };
bool helperBool[MAXWORDLENGTH] = { false, false, false, false, false, false };

// functions
int playOneRound(const char words[maxWords][MAXWORDLENGTH + 1], int nWords, int wordnum);
int isValidWord(char word[]);
int isInDictionary(const char dictionary[][7], char word[]);
int numGolds(const char targetWord[], char guess[]);
int numSilvers(const char targetWord[], char guess[]);

int main()
{
	int n = getWords(w, 9000, WORDFILENAME);
	if (n < 1) // if no words in text file
		cout << "No words were loaded, so I can't play the game." << endl;
	else
	{
		int random; // declare integer random

		double totalScore = 0; // initialize total score as a double 0
		double avg; // declare double average called avg
		int minScore; // declare integer representing the minimum score
		int maxScore; // declare integer representing the maximum score

		int rNum; // declare integer representing the round number
		int rounds; // declare integer for total number of rounds
		cout << "How many rounds do you want to play? "; // ask how many rounds user wants to play
		cin >> rounds; // user inputs number of rounds
		cin.ignore(10000, '\n');

		if (rounds <= 0) // if rounds is 0 or negative
		{
			cout << "The number of rounds must be positive.";
		}
		else
		{
			for (rNum = 0; rNum < rounds; rNum++) // iterate through each round
			{
				random = randInt(0, nWords - 1); // set random to random integer from 0 to 7264

				char answer[MAXWORDLENGTH + 1]; // initialize answer array
				strcpy(answer, w[random]); // copy random word in w to answer
				cout << "\nRound " << rNum + 1 << endl; // prints round number
				cout << "The hidden word is " << strlen(answer) << " letters long" << endl; // prints how long hidden word is
				int t = playOneRound(w, n, random); // calls playOneRound function

				if (rNum == 0) // if it is the first round
				{
					minScore = t; // minScore is number of tries first round
					maxScore = t; // maxScore is number of tries first round
				}

				totalScore += t; // add tries to totalScore
				avg = totalScore / (rNum + 1); // calculate average

				if (t < minScore) // if tries is less than minScore
					minScore = t; // set minScore to tries
				if (t > maxScore) // if tries is greater than maxScore
					maxScore = t; // set maxScore to tries

				// round to two decimal places
				cout.setf(ios::fixed);
				cout.precision(2);

				cout << "You got it in " << t << " tries." << endl; // print number of tries
				cout << "Average: " << avg << ", minimum: " << minScore << ", maximum: " << maxScore << endl; // print average, min and max score
			}
		}
	}
}

// function to play one round of game
int playOneRound(const char words[maxWords][MAXWORDLENGTH + 1], int nWords, int wordnum)
{
	if (nWords < 0 || wordnum < 0 || wordnum >= nWords) // returns -1 if number of words or wordnum is negative, or if wordnum is greater than or equal to number of words
		return -1;

	char guess[100]; // initialize guess
	char answer[MAXWORDLENGTH + 1]; // initialize
	strcpy(answer, words[wordnum]);

	int gold; // declare integer gold
	int silver; // declare integer silver

	int tries = 0; // set tries to 0
	
	// get user input
	cout << "Probe word: "; 
	cin.getline(guess, 100);

	if (isValidWord(guess) == -1) // ensure word is 
		cout << "Your probe word must be a word of 4 to 6 lower case letters." << endl;
	else if (isInDictionary(words, guess) == -1)
		cout << "I don't know that word" << endl;
	else if (strcmp(guess, answer) == 0) // if guess is the hidden word
		tries++;
	else
	{
		gold = numGolds(answer, guess);
		silver = numSilvers(answer, guess);
		cout << "Golds: " << gold << ", Silvers: " << silver << endl;
		tries++;
	}

	while (strcmp(answer, guess) != 0)
	{
		cout << "Probe word: ";
		cin.getline(guess, 100);

		if (isValidWord(guess) == -1)
			cout << "Your probe word must be a word of 4 to 6 lower case letters." << endl;
		else if (isInDictionary(words, guess) == -1)
			cout << "I don't know that word" << endl;
		else if (strcmp(guess, answer) == 0) // if guess is the hidden word
			tries++;
		else
		{
			gold = numGolds(answer, guess);
			silver = numSilvers(answer, guess);
			cout << "Golds: " << gold << ", Silvers: " << silver << endl;
			tries++;
		}
	}
	return tries;
}

// ensures word is a valid word
// returns 0 if word is a valid word, returns -1 if word is invalid
int isValidWord(char word[])
{
	if (strlen(word) < 4 || strlen(word) > 6) // returns -1 if length of word is less than 4 or greater than 6
		return -1;

	for (int i = 0; i != strlen(word); i++) // iterate through word
	{
		if (!islower(word[i])) // returns -1 if there is an uppercase letter
			return -1;

		else if (!isalpha(word[i])) // returns -1 if character in word is not a letter
			return -1;
	}

	return 0;
}

int isInDictionary(const char dictionary[][7], char word[])
{
	for (int i = 0; i < maxWords; i++) // iterate through all words in dictionary (list of words given)
	{
		if (strcmp(dictionary[i], word) == 0) // if word matches a word in dictionary
			return 0;
	}
	return -1;
}

// count number of golds
int numGolds(const char targetWord[], char guess[])
{
	int minLength;
	if (strlen(targetWord) < strlen(guess))
		minLength = strlen(targetWord);
	else
		minLength = strlen(guess);

	int numGold = 0; // set number of golds to zero
	for (int i = 0; i < minLength; i++) // iterate through both words
	{
		if (targetWord[i] == guess[i]) // if letters in targetWord and guess are equal at the same position
		{
			numGold++; // increment number of golds
			help[i] = 'Y'; // set that position of help array to 'Y'
			helperBool[i] = true;
		}

	}
	return numGold; // return number of golds
}

// count number of silvers
int numSilvers(const char targetWord[], char guess[])
{
	int numSilver = 0; // set number of silvers to zero

	for (int i = 0; i != strlen(guess); i++) // iterate through guess array
	{
		if (help[i] == 'N')
		{
			for (int j = 0; j != strlen(targetWord); j++) // iterate through targetWord array
			{
				if (targetWord[j] == guess[i] && !helperBool[j]) // if targetWord at j is the same as guess at i, and help array is N (to avoid gold duplicates)
				{
					numSilver++; // increment number of silver
					help[i] = 'Y'; // set help at j to Y
					helperBool[j] = true;
					break; // break out of for loop
				}
			}
		}

	}

	for (int k = 0; k < MAXWORDLENGTH + 1; k++) // iterate through help array
	{
		help[k] = 'N'; // reset array help to all N values
	}

	for (int l = 0; l < MAXWORDLENGTH; l++)
	{
		helperBool[l] = false;
	}

	return numSilver; // return number of silvers
}