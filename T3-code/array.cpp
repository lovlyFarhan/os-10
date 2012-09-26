/*
 * Arrays in C++
 * For use in CPSC 457 tutorials
 * Instructor: Prof. Mea Wang
 */

#include <iostream>

#define arraySize 10

using namespace std;

int main(void)
{
    int array[arraySize];  // static allocation of an array
    int i, total = 0; 
    
    // Initialize the array
    for (i = 0; i < arraySize; i++)
    {
        array[i] = i; 
    }

    // What's in the array now?
    for (i = 0; i < arraySize; i++)
    {
        cout << i << " : " << array[i] << endl;
    }

    // Add up the numbers in the array
    for (i = 0; i < arraySize; i++)
    {
        total += array[i];  
    }

    cout << "The total is: " << total << endl;

    return 0;
}