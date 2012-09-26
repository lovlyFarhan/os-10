/*
 * Parsing command-line arguments in C.
 * For use in CPSC 457 tutorials
 * Instructor: Prof. Mea Wang
 */

#include <stdio.h>

int main(int argc, char * argv[])
{
    int i;
    
    printf( "Number of parameters is: %d\n", argc);

    for (i = 0; i < argc; i++)
    {
        printf( "Parameter %d is: %s\n", i, argv[i]);
    }
        
    return 0;
}
