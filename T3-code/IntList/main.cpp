/*
 * An example of C++ classes and objects.
 * For use in CPSC 457 tutorials
 * Instructor: Prof. Mea Wang
 */
 
#include "IntList.h"

int main(void)
{
    IntList list(3);
    if (list.append(10))
    {
        if (list.append(20))
        {
            if (list.append(30))
            {
                if (list.append(40));
                else
                    cout << "Failed to append 40" << endl;                
            }
            else 
                cout << "Failed to append 30" << endl;
        }
        else
            cout << "Failed to append 20" << endl;
    }
    else
        cout << "Failed to append 10" << endl;

    list.print();
}
