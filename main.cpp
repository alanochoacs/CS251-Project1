#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Function prototypes at the top of the file for function decomposition

/**
 * Print instructions for using the program.
 */
void printMenu();
/**
 * Returns the 0-based index in the English alphabet where `c` appears,
 * or -1 if `c` is not an uppercase letter in the alphabet.
 *
 * For example:
 *  - `findIndexInAlphabet('A')` returns 0
 *  - `findIndexInAlphabet('D')` returns 3
 *  - `findIndexInAlphabet('+')` returns -1
 *  - `findIndexInAlphabet('a')` returns -1
 */
int findIndexInAlphabet(char c);
/**
 * Returns `c` rotated by `amount` many characters forward. If we run out
 * of letters in the alphabet, wrap around back to 'A'. This method
 * assumes that `c` is an uppercase letter.
 * For example:
 *  - `rot('A', 0)` returns 'A'
 *  - `rot('A', 1)` returns 'B'
 *  - `rot('Z', 1)` returns 'A'
 *  - `rot('A', 10)` returns 'K'
 *  - `rot('J', 25)` returns 'I'
 */
char rot(char c, int amount);
/**
 * Returns a new string in which every character has been rotated by `amount`
 * letters. Lowercase letters are converted to uppercase and rotated.
 * Non-alphabetic characters are left alone.
 * For example:
 *  `rot("A", 0)` returns "A"
 *  `rot("AA", 3)` returns "DD"
 *  `rot("HELLO", 0)` returns "HELLO"
 *  `rot("HELLO", 1)` returns "IFMMP"
 *  `rot("oh HEL-LO!", 1)` returns "PI IFM-MP!"
 */
string rot(string line, int amount);
/**
 * Asks the users for a line of text and amount of rotation
 * Then calls the rotation method (rot(string, int))
 * Lastly, the rotated user inputed line is printed out
 */
void encryptCaesar();
/**
 * Asks the users for a line of text that will be rotated
 * Next, the user is asked for a key that will used as a key for the rotation
 * If the key is smaller then the line of text, it will loop back to the front of the key 
 * Then calls the rotation method (rot(c, int)) for every character using the key as rotation
 * Lastly, the rotated user inputed line is printed out
 */
void encryptVigenere();
/**
 * Asks the user for a caesar-cipher encrypted line of text and calls lineToVector,
 * Checks if the parameter file was opened
 * Then loops through all possible rotations and elements in the word vector
 * Checks all words of the dictionary against all possible rotations
 * If a rotation has 1/2 of the words in the vector match in the dictionary
 * Then that possible decryption is printed out
 */
void decryptCaesar(ifstream& infile);
/**
 * Takes a string vector by reference and a line of text
 * Parses the string into seperate words
 * All non-alpha characters are not included and all are upper case
 */
void lineToVector(vector<string>& words, string line);

int main() {
    string command;

    cout << "Welcome to Ciphers!" << endl;
    cout << "-------------------" << endl;
    cout << endl;

    do {
        printMenu();
        cout << endl
             << "Enter a command (case does not matter): ";

        // Use getline for all user input to avoid needing to handle
        // input buffer issues relating to using both >> and getline
        getline(cin, command);
        cout << endl;

        if (command == "C" || command == "c") {
            encryptCaesar();
        }

        if (command == "V" || command == "v") {
            encryptVigenere();
        }

        if (command == "D" || command == "d") {
            // Load the dictionary (dictionary.txt) in the main method
            ifstream infile("dictionary.txt");
            decryptCaesar(infile);
        }

        cout << endl;

    } while (!(command == "x" || command == "X"));

    return 0;
}

void printMenu() {
    cout << "Ciphers Menu" << endl;
    cout << "------------" << endl;
    // Caesar Ciphers
    cout << "C - Encrypt with Caesar Cipher" << endl;
    cout << "D - Decrypt Caesar Cipher" << endl;
    // Vigenere
    cout << "V - Encrypt with Vigenère" << endl;
    // Utility
    cout << "X - Exit Program" << endl;
}

int findIndexInAlphabet(char c) {
    return ALPHABET.find(c, 0);
}

char rot(char c, int amount)  {
    // ASCII for capital letters (65 - 90)
    c = toupper(c);
    int character = (int) c; // convert char to int
    character += amount; // add rotation

    // check characters for overflow out of capital letters
    if (character > 90) {
        int temp = character - 91;
        character = 65 + temp;
    }

    c = (char) character; // converts int back to char
    return c;
}

string rot(string line, int amount) {
    for (unsigned int i = 0; i < line.size(); i++) {
        if (isalpha(line.at(i))) {
            line.at(i) = rot(toupper(line.at(i)), amount);
        }
    }
    return line;
}

void encryptCaesar() {
    cout << "Enter the text to encrypt:";
    string line;
    getline(cin, line);
    cout << endl;

    cout << "Enter the number of characters to rotate by:";
    string shift;
    getline(cin, shift);
    cout << endl;

    cout << rot(line, stoi(shift)) << endl;
}

void encryptVigenere() {
    cout << "Enter text to encrypt:";
    string line;
    getline(cin, line);
    cout << endl;

    cout << "Enter the Vigenère key:";
    string key;
    getline(cin, key);
    cout << endl;

    int key_index = 0; // tracks position of the current key
    for (unsigned int i = 0; i < line.size(); i++) {
        if (key.at(key_index) == ' ') key_index++;
        if (key_index >= key.size()) key_index = 0;
        if (isalpha(line.at(i))) {
            line.at(i) = rot(toupper(line.at(i)), findIndexInAlphabet(toupper(key.at(key_index)))); // rotates based on index in alphabet
            key_index++;
            if (key_index >= key.size()) key_index = 0; // wraps the key to the begining
        }
    }
    cout << line << endl;
}

void decryptCaesar(ifstream& infile) {
    // Ask for text to decrypt (“Enter the text to Caesar-cipher decrypt:”)
    cout << "Enter the text to Caesar-cipher decrypt:";
    string line;
    getline(cin, line);
    cout << endl;

    // Break the text into a vector of words (separated by spaces), converting lowercase to uppercase and removing non-letters.
    vector<string> words;

    lineToVector(words, line);

    string wordFromDictionary;
    vector<int> acceptableShifts;

    if (infile.is_open()) {
            for (int shift = 1; shift <= 26; shift++) { // check all shifts
                unsigned int matches = 0;
                for (int index = 0; index < words.size(); index++) { // check the whole vector
                    infile.clear();
                    infile.seekg(0);
                    while (getline(infile, wordFromDictionary)) {
                        string rotation = rot(words.at(index), shift);

                        if (wordFromDictionary.compare(rotation) == 0) {
                            matches++;
                            if (matches > (words.size() / 2)) { // if 1/2 of words in current rotation are in the dictionary
                                bool found = false;
                                for (int i = 0; i < acceptableShifts.size(); i++) { 
                                    if (acceptableShifts.at(i) == shift) {
                                        found = true;
                                    }
                                }
                                if (!found) {
                                    acceptableShifts.push_back(shift);
                                }
                            }
                        }
                    }
                }
            }
        infile.close(); // Close the file when done

        if (acceptableShifts.size() > 0) { // if there are possible rotations, print them out
            for (int i = 0; i < acceptableShifts.size(); i++) {
                cout << rot(line, acceptableShifts.at(i)) << endl;
            }
        } else { 
            cout << "No good decryptions found" << endl;
        }
    } else {
        cout << "Unable to open file for reading." << endl;
    }
}

void lineToVector(vector<string>& words, string line) {
    if (line.find(' ', 0) == string::npos) { // if there are no spaces
        words.push_back(line);
    } else { // spaces
        int start = 0; // tracks the start of a new word
        for (int i = 0; i < line.size(); i++) {
            if (i + 1 == line.size()) { // if at the end, push back the word
                words.push_back(line.substr(start, i));
            }
            if (line.at(i) == ' ' && line.at(i + 1) != ' ') { // if i is a space and next isn't a space (thus a new word)
                words.push_back(line.substr(start, i - start));
                start = i + 1;
            }
        }
    }
    
    for (int i = 0; i < words.size(); i++)  { // removes all characters that are not alphabetical and uppercases letters in the vector
        for (int j = 0; j < words.at(i).size(); j++) {
            if (isalpha(words.at(i).at(j))) {
                words.at(i).at(j) = toupper(words.at(i).at(j));
            } else {
                words.at(i).erase(j, 1);
                j--;
            }
        }
    }
}