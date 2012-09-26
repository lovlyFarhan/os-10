/*
 * File I/O in C++.
 * For use in CPSC 457 tutorials
 * Instructor: Prof. Mea Wang
 */

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char * argv[]) {
    ifstream infile;
    // Check for the right number of parameters
    if (argc != 2) {
        cout << "Usage: readFile <filename>" << endl;
        return -1;
    }

    // Open the file for reading
    infile.open(argv[1]);
    
    // Read one line from the file
    string line;
    while(infile.good()){
        getline(infile, line);
        if (infile.good()) {
            cout << line << endl; 
        } 
    }

    // Close the file
    infile.close();
    return 0;
}

