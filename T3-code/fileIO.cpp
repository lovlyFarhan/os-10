/*
 * File I/O in C++.
 * For use in CPSC 457 tutorials
 * Instructor: Prof. Mea Wang
 */

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char * argv[])
{
    ifstream infile;
    cout << argc << endl; 
    // Open the file again for reading
    infile.open(argv[1]);
    
    // Read one line from the file
    string line;
    getline(infile, line);
    
    // Print the line to standard output
    cout << line << endl;

    // Close the file
    infile.close();
    return 0;}

