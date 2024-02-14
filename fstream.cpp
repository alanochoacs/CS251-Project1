#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    // Writing to a file
    ofstream outfile("example.txt"); // Create an ofstream object for writing
    if (outfile.is_open()) {
        outfile << "Hello, this is a test file.\n"; // Write to the file
        outfile.close(); // Always close the file when done
    } else {
        cout << "Unable to open file for writing." << endl;
    }

    // Reading from a file
    ifstream infile("example.txt"); // Create an ifstream object for reading
    string line;
    if (infile.is_open()) {
        while (getline(infile, line)) { // Read lines from the file
            cout << line << '\n';
        }
        infile.close(); // Close the file when done
    } else {
        cout << "Unable to open file for reading." << endl;
    }

    return 0;
}
