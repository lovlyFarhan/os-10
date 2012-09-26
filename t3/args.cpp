/*
 * Parsing command-line arguments in C++.
 * For use in CPSC 457 tutorials
 * Instructor: Prof. Mea Wang
 */

#include <iostream>

using namespace std;

int main(int argc, char * argv[])
{
    cout << "Number of parameters is: " << argc << endl;

    for (int i = 0; i < argc; i++)
    {
        cout << "Parameter " << i << " is: " << argv[i] << endl;
    }
    
    return 0;
}

