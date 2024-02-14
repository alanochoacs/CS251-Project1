#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    string line;
    cout << "Enter a line of text: ";
    getline(cin, line); // Reading a line of text

    istringstream iss(line);
    string word;
    int wordCount = 0;

    // Counting words by reading each word from the line
    while (iss >> word) {
        wordCount++;
    }

    cout << "Number of words in the entered line: " << wordCount << endl;

    return 0;
}
