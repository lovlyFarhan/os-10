/*
 * An example of C++ classes and objects.
 * For use in CPSC 457 tutorials
 * Instructor: Prof. Mea Wang
 */

#include <iostream>

using namespace std;

class IntList {
public:
    /* Constructor: initialize the list to a specific size
    */
    IntList (int s);
    
    /* Append an item to the end of the list
     @param item The iterm to append
     @return Whether the append is successful.
     */
    bool append (int item);
    
    /* Display the list.
     Note: const enforces that this function will not modify any variable
     */
    void print () const;

private:
    int * items;                // items will point to the dynamically allocated array
    int numItems;               // number of items currently in the list
    int size;                   // the current size of the array
};
