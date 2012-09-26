/*
 * File I/O in C.
 * For use in CPSC 457 tutorials
 * Instructor: Prof. Mea Wang
 */

#include <stdio.h>

int main(int argc, char * argv[])
{
    FILE * fd;
    char str[10];
    
    // Check for the right number of parameters
    if (argc != 2)
    {
        printf("Usage: fileIO <filename>\n");
        return -1;
    }

    // Open the file for writing
    fd = fopen (argv[1], "w");
    
    // Write to the file
    fprintf(fd, "Hello\n");
    
    // Close the file
    fclose(fd);

    // Open the file again for reading
    fd = fopen (argv[1], "r");
    
    // Read one line from the file
    fscanf(fd, "%s", str);
    
    // Print the line to standard output
    printf("%s\n", str);
    
    // Clode the file
    fclose(fd);
        
    return 0;
}
