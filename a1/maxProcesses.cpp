/*
 * Creating a number of processes
 * For use in CPSC 457
 * Instructor: Prof. Mea Wang
 */


#include <unistd.h>  // for fork()
#include <iostream>  // for cout
#include <string.h>  // for strerror()
#include <errno.h>   // for errno
#include <stdlib.h>  // for atoi()

using namespace std;

int main(int argc, char * argv[])
{
    // Check for the right number of parameters
    if (argc != 2)
    {
        cout << "Usage: maxProcesses <number of processes>" << endl;
        return -1;
    }
    
    // Get the number of processes from the command line
    int num = atoi(argv[1]);
 
    // Get the parent PID
    pid_t ppid = getpid();
 
    // Forking processes
    for (int i = 0; i < num; i++)
    {
        // Only the parent will fork a new process
        if (getpid() == ppid)
        {
            cout << "Creating process " << i << endl;
            pid_t pid = fork();
            
            // Check the status of fork()
            if (pid < 0)
            {
                cout << "Failed to create: " << strerror(errno) << endl;
            }
        }
    }
    
    return 0;
}
