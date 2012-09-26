/*
 * Arrays in C. 
 * For use in CPSC 457 tutorials
 * Instructor: Prof. Mea Wang
 */

#include <stdio.h>

#define arraySize 10

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
        printf("%i : %i\n", i, array[i]);
    }

    // Add up the numbers in the array
    for (i = 0; i < arraySize; i++)
    {
        total += array[i];  
    }

    printf("The total is: %i: \n", total);

    return 0;
}

