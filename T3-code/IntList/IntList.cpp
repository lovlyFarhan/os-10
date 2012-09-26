/*
 * An example of C++ classes and objects.
 * For use in CPSC 457 tutorials
 * Instructor: Prof. Mea Wang
 */
 
#include "IntList.h"

IntList::IntList(int s)
{
    items = new int[s];
    numItems = 0;
    size = s;
}

bool IntList::append(int item)
{
    // If the list if already full, increase the list size by 5
    if (numItems >= size)
    {
        return false;
    }
    
    // Append the item to the list
    items[numItems] = item;
    
    // Increase the item count
    numItems++;

    return true;
}

void IntList::print() const
{
	for (int i = 0; i < numItems; i++)
	{
		cout << items[i] << endl;
	}
}